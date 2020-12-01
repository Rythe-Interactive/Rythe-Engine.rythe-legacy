#pragma once
#include <scripting/hostfxr/hostfxr_helper.hpp>
#include <scripting/util/to_char_t.hpp>


namespace legion::scripting {
class CSharpProviderBase
{
public:
    virtual bool onHostFXRRegister(HostFXRLoader* loader) = 0;
    virtual ~CSharpProviderBase() = default;

protected:
     Assembly engineFunction(
            HostFXRLoader* loader,
            const char_t* name)
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
        const char_t* classname;
        const char_t* assembly_name = TO_CHAR_T("dotnetlegion");
};
}
