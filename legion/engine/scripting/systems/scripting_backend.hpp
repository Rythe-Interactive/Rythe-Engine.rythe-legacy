

#include <core/engine/system.hpp>
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
            const char_t* name)
        {
            std::basic_string<char_t> delegateName = TO_CHAR_T("Legion.Engine+") +
                std::basic_string<char_t>(name) + TO_CHAR_T("Fn, dotnetlegion");

            return loader->loadAssembly(
                TO_CHAR_T("..\\..\\scripting_frontend\\bin\\Debug\\net5.0\\dotnetlegion.dll"),
                TO_CHAR_T("Legion.Engine, dotnetlegion"),
                name,
                delegateName.c_str()
            );
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
