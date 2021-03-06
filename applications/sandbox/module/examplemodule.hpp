#pragma once
#include <core/core.hpp>

#include "../systems/mysystem.hpp"
#include "../systems/examplesystem.hpp"
#include "../systems/simplecameracontroller.hpp"
#include "../systems/gui_test.hpp"
#include "../systems/diviner_physics_test_system.hpp"
#include "../systems/physx_test_system.hpp"
#include "../systems/physics_debugging_system.hpp"

class ExampleModule : public legion::Module
{
public:
    virtual void setup() override
    {
        using namespace legion;
        app::WindowSystem::requestWindow(
            ecs::world_entity_id,
            math::ivec2(1920, 1080),
            "LEGION Engine",
            "Legion Icon",
            nullptr,
            nullptr,
            0
        );

        //reportSystem<MySystem>();
        reportSystem<SimpleCameraController>();
        reportSystem<ExampleSystem>();
        reportSystem<GuiTestSystem>();
        reportSystem<physics::PhysicsDebuggingSystem>();

        registerComponentType<self_destruct_component>();
    }
};
