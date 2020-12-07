#pragma once
#include <core/core.hpp>
#include <scripting/providers/provider_base.hpp>

namespace legion::scripting {

    class CSharpBasicMemoryProvider : public CSharpProviderBase
    {
    public:
        bool onHostFXRRegister(HostFXRLoader* loader) override
        {
            classname = TO_CHAR_T("GetOut");
            auto register_filesystem_functions =  engineFunction(loader, TO_CHAR_T("RegisterFreeFn"));

            register_filesystem_functions.invoke<void>
            (
                &wrapped_clean_array
            );
            return true;
        }

    protected:
     
        static void wrapped_clean_array(byte* ptr)
        {
            delete[] ptr;
        }

    };
}
