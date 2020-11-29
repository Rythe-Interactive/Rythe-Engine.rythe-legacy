#pragma once
#include <core/engine/module.hpp>
#include <core/types/primitives.hpp>
#include <scripting/systems/scripting_backend.hpp>

namespace legion::scripting
{
    class ScriptModule : public core::Module
    {
    public:
        void setup() override
        {
            addProcessChain("Scripting");
            reportSystem<ScriptingBackendSystem>();
        }

        core::priority_type priority() override
        {
            return 101;
        }

    };
}
