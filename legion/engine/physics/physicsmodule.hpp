#pragma once

#include <core/core.hpp>
#include <physics/systems/physicssystem.hpp>
#include <physics/components/physics_component.hpp>
#include <physics/components/rigidbody.hpp>
#include <physics/mesh_splitter_utils/mesh_splitter.hpp>
#include <physics/components/fracturer.hpp>
#include <physics/components/fracturecountdown.hpp>
#include <physics/systems/physics_test_system.hpp>

namespace legion::physics
{
    class PhysicsModule : public Module
    {

    public:

        virtual void setup() override
        {
            createProcessChain("Physics");
            reportSystem<PhysicsSystem>();
            registerComponentType<physicsComponent>();
            registerComponentType<rigidbody>();
            registerComponentType<identifier>();
            registerComponentType<MeshSplitter>();
            registerComponentType<Fracturer>();
            registerComponentType<FractureCountdown>();
            registerComponentType<ObjectToFollow>();
            //reportComponentType <addRB>();

            lgn::log::debug("PhysicsModule setup");
        }

        virtual priority_type priority() override
        {
            return 20;
        }

    };

}


