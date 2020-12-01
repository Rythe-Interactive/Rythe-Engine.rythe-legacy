#pragma once

#include <core/engine/system.hpp>
#include <core/logging/logging.hpp>
#include <core/core.hpp>
#include <scripting/hostfxr/hostfxr_helper.hpp>
#include <scripting/providers/provider_base.hpp>
#include <scripting/providers/provider_basic_memory.hpp>
#include <scripting/providers/provider_core_ecs.hpp>
#include <scripting/providers/provider_core_fs.hpp>
#include <scripting/providers/provider_core_logging.hpp>
#include <scripting/util/to_char_t.hpp>


namespace legion::scripting {

    namespace detail
    {
        inline auto internalCall(HostFXRLoader* loader,
            const char_t* name,const char_t* classname = TO_CHAR_T("Engine"), const char_t* assembly_name = TO_CHAR_T("dotnetlegion"))
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
                TO_CHAR_T("..\\..\\scripting_frontend\\bin\\Debug\\net5.0\\dotnetlegion.dll"),
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
    public:

        void setup() override
        {
            createProcess<&ScriptingBackendSystem::update>("Scripting");
        }



        bool prepared = false;
        void update(time::span dt)
        {
            if(!prepared){
                HostFXRLoader loader("hostfxr.dll");
                loader.initHostFXR(TO_CHAR_T("dotnet.runtimeconfig.json"));

                CSharpCoreEcsProvider::registry = m_ecs;

                std::pair<const char *,std::unique_ptr<CSharpProviderBase>> providers[] = {
                    {"GetOut Provider",std::make_unique<CSharpBasicMemoryProvider>()},
                    {"Entity Provider",std::make_unique<CSharpCoreEcsProvider>()},
                    {"Logging Provider",std::make_unique<CSharpCoreLoggingProvider>()},
                    {"Filesystem Provider",std::make_unique<CSharpCoreFilesystemProvider>()}
                };

                for (auto& [str,provider] : providers)
                {
                    log::trace("Assembling Registries for: {}",str);
                    provider->onHostFXRRegister(&loader);
                }
                m_init_assembly = detail::internalCall(&loader, TO_CHAR_T("Initialize"));
                m_update_assembly = detail::internalCall(&loader, TO_CHAR_T("Update"));

                m_init_assembly.invoke<void>();
                prepared = true;
            }
            m_update_assembly.invoke<void>(static_cast<float>(dt));
        }

    };
}
