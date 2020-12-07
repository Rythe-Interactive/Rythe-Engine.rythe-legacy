#pragma once
#include <core/core.hpp>
#include <scripting/providers/provider_base.hpp>

namespace legion::scripting {

    class CSharpCoreLoggingProvider : public CSharpProviderBase
    {
    public:
        bool onHostFXRRegister(HostFXRLoader* loader) override
        {
            classname = TO_CHAR_T("Log");
            auto register_logging_functions =  engineFunction(loader, TO_CHAR_T("RegisterLogFns"));

            register_logging_functions.invoke<void>(
                &wrapped_err,
                &wrapped_warn,
                &wrapped_info,
                &wrapped_debug,
                &wrapped_trace
            );

            return true;
        }

        static void wrapped_err(const char * str)
        {
            log::error("[C#] {}",str);
        }
        static void wrapped_warn(const char* str)
        {
            log::warn("[C#] {}",str);
        }
        static void wrapped_info(const char * str)
        {
            log::info("[C#] {}",str);
        }
        static void wrapped_debug(const char * str)
        {
            log::debug("[C#] {}",str);
        }
        static void wrapped_trace(const char * str)
        {
            log::trace("[C#] {}",str);
        }
    };
}
