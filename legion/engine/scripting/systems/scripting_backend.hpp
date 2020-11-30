#pragma once

#include <core/engine/system.hpp>
#include <core/logging/logging.hpp>
#include <scripting/hostfxr/hostfxr_helper.hpp>

#ifdef _MSC_VER
#define TO_CHAR_T(X) L ## X
#else
#define TO_CHAR_T(X) X
#endif

namespace legion::scripting {

    namespace detail
    {
        inline auto engineFunction(HostFXRLoader* loader,
            const char_t* name,const char_t* classname = TO_CHAR_T("Engine"))
        {
            std::basic_string<char_t> delegateName =
                TO_CHAR_T("Legion.") +
                std::basic_string<char_t>(classname) +
                TO_CHAR_T("+") +
                std::basic_string<char_t>(name) +
                TO_CHAR_T("Fn, dotnetlegion");

            std::basic_string<char_t> assemblyName =
                TO_CHAR_T("Legion.") +
                std::basic_string<char_t>(classname) +
                TO_CHAR_T(", dotnetlegion");

            return loader->loadAssembly(
                TO_CHAR_T("..\\..\\scripting_frontend\\bin\\Debug\\net5.0\\dotnetlegion.dll"),
                assemblyName.c_str(),
                name,
                delegateName.c_str()
            );
        }

        void wrapped_err(const char * str)
        {
            core::log::error("[C#] {}",str);
        }
        void wrapped_warn(const char* str)
        {
            core::log::warn("[C#] {}",str);
        }
        void wrapped_info(const char * str)
        {
            core::log::info("[C#] {}",str);
        }
        void wrapped_debug(const char * str)
        {
            core::log::debug("[C#] {}",str);
        }
        void wrapped_trace(const char * str)
        {
            core::log::trace("[C#] {}",str);
        }

    }


    class ScriptingBackendSystem : public core::System<ScriptingBackendSystem>
    {
        Assembly m_init_assembly = nullptr;
        Assembly m_update_assembly = nullptr;
        Assembly m_shutdown_assembly = nullptr;
    public:

        void setup() override
        {
            createProcess<&ScriptingBackendSystem::update>("Scripting");
        }



        bool prepared = false;
        void update(core::time::span dt)
        {
            if(!prepared){
                HostFXRLoader loader("hostfxr.dll");
                loader.initHostFXR(TO_CHAR_T("dotnet.runtimeconfig.json"));


                auto register_logging_functions =  detail::engineFunction(&loader, TO_CHAR_T("RegisterLogFns"),TO_CHAR_T("Log"));

                register_logging_functions.invoke<void>(
                    &detail::wrapped_err,
                    &detail::wrapped_warn,
                    &detail::wrapped_info,
                    &detail::wrapped_debug,
                    &detail::wrapped_trace
                );


                m_init_assembly = detail::engineFunction(&loader, TO_CHAR_T("Initialize"));
                m_update_assembly = detail::engineFunction(&loader, TO_CHAR_T("Update"));
                m_shutdown_assembly = detail::engineFunction(&loader, TO_CHAR_T("Shutdown"));

                m_init_assembly.invoke<void>();
                prepared = true;
            }
            m_update_assembly.invoke<void>(0.0f);
        }

    };
}
