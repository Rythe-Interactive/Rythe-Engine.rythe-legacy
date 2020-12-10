#pragma once

#include <core/engine/system.hpp>
#include <core/logging/logging.hpp>
#include <core/core.hpp>
#include <scripting/events/csnotifyone.hpp>
#include <scripting/hostfxr/hostfxr_helper.hpp>
#include <scripting/providers/provider_base.hpp>
#include <scripting/providers/provider_basic_memory.hpp>
#include <scripting/providers/provider_core_ecs.hpp>
#include <scripting/providers/provider_core_eventbus.hpp>
#include <scripting/providers/provider_core_fs.hpp>
#include <scripting/providers/provider_core_logging.hpp>
#include <scripting/util/to_char_t.hpp>


namespace legion::scripting {

    namespace detail
    {
        inline auto internalCall(HostFXRLoader* loader,
            const char_t* name, const char_t* classname = TO_CHAR_T("Engine"), const char_t* assembly_name = TO_CHAR_T("dotnetlegion"))
        {
            std::basic_string<char_t> delegateName =
                TO_CHAR_T("Legion.") +
                std::basic_string<char_t>(classname) +
                TO_CHAR_T("+") +
                std::basic_string<char_t>(name) +
                TO_CHAR_T("Fn, ") +
                std::basic_string<char_t>(assembly_name);

            std::basic_string<char_t> assemblyName =
                TO_CHAR_T("Legion.") +
                std::basic_string<char_t>(classname) +
                TO_CHAR_T(", ") +
                std::basic_string<char_t>(assembly_name);

            return loader->loadAssembly(
                TO_CHAR_T("..\\..\\legion\\engine\\scripting_frontend\\bin\\Debug\\net5.0\\dotnetlegion.dll"),
                assemblyName.c_str(),
                name,
                delegateName.c_str()
            );
        }
    }


    class ScriptingBackendSystem : public core::System<ScriptingBackendSystem>
    {
        Assembly m_init_assembly = nullptr;
        Assembly m_update_assembly = nullptr;
        std::queue<events::event_base*> m_eventQueue;

        //sorry glyn but your locks are too buggy for me
        mutable std::mutex m_lock;
        CSharpCoreEventBusProvider* ebp = nullptr;

        std::vector<std::pair<std::string, id_type>> events = {
            std::make_pair("core::exit", events::exit::id),
            std::make_pair("core::component_creation<transform>",events::component_creation<transform>::id),
            std::make_pair("notify_one",csnotifyone::id)
        };

    public:

        void setup() override
        {
            createProcess<&ScriptingBackendSystem::update>("Scripting");
            m_eventBus->bindToAllEvents(delegate<void(events::event_base*, size_type)>::create<ScriptingBackendSystem, &ScriptingBackendSystem::enqueueEvent>(this));
        }

        void enqueueEvent(events::event_base* ptr, size_type evsize)
        {
            std::lock_guard<std::mutex> guard(m_lock);
            byte* buffer = new byte[evsize];
            memcpy(buffer, ptr, evsize);

            m_eventQueue.emplace(reinterpret_cast<events::event_base*>(buffer));
        }

        bool prepared = false;
        void update(time::span dt)
        {
            if (!prepared) {
                HostFXRLoader loader("hostfxr.dll");
                loader.initHostFXR(TO_CHAR_T("dotnet.runtimeconfig.json"));

                CSharpCoreEcsProvider::registry = m_ecs;

                ebp = new CSharpCoreEventBusProvider(events);
                std::pair<const char*, std::unique_ptr<CSharpProviderBase>> providers[] = {
                    {"GetOut Provider",std::make_unique<CSharpBasicMemoryProvider>()},
                    {"EventBus Provider",std::unique_ptr<CSharpCoreEventBusProvider>(ebp)},
                    {"Entity Provider",std::make_unique<CSharpCoreEcsProvider>()},
                    {"Logging Provider",std::make_unique<CSharpCoreLoggingProvider>()},
                    {"Filesystem Provider",std::make_unique<CSharpCoreFilesystemProvider>()}
                };
                for (auto& [str, provider] : providers)
                {
                    log::trace("Assembling Registries for: {}", str);
                    provider->onHostFXRRegister(&loader);
                }
                m_init_assembly = detail::internalCall(&loader, TO_CHAR_T("Initialize"));
                m_update_assembly = detail::internalCall(&loader, TO_CHAR_T("Update"));

                m_init_assembly.invoke<void>();
                prepared = true;
            }
            m_update_assembly.invoke<void>(static_cast<float>(dt));

            {
                std::lock_guard<std::mutex> guard(m_lock);
                while (!m_eventQueue.empty())
                {
                    events::event_base* event = m_eventQueue.front();
                    m_eventQueue.pop();
                    CSharpCoreEventBusProvider::onEvent(event);
                }
            }
        }
    };
}
