#pragma once
#include <core/core.hpp>

#include <physics/components/rigidbody.hpp>
#include <physics/components/physics_component.hpp>

#include <physics/diviner/systems/physicssystem.hpp>
#include <physics/diviner/components/dvr_internal_physics_component.hpp>
#include <physics/diviner/components/dvr_internal_rigidbody.hpp>
#include <physics/physx/systems/physx_physics_system.hpp>

namespace legion::physics
{
    class PhysicsModule : public Module
    {
    public:

        virtual void setup() override
        {
            createProcessChain("Physics");
            reportSystem<PhysicsSystem>();
            registerComponentType<diviner::physics_component>();
            registerComponentType<diviner::rigidbody>();
            registerComponentType<identifier>();

            reportSystem<PhysXPhysicsSystem>();
            registerComponentType<physicsComponent>();
            registerComponentType<rigidbody>();
        }

        virtual priority_type priority() override
        {
            return 20;
        }
    };
}
