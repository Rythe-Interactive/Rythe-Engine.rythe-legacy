#pragma once
#include "unit_test.hpp"
#include <physics/components/rigidbody.hpp>

static void TestRigidbody()
{
    using namespace legion;

    physics::rigidbody rigidbody;
    #define NOT !

    LEGION_SUBTEST("Rigidbody Read and Write")
    {
        physics::RigidbodyData& rbData = rigidbody.rigidbodyData;

        rbData.setMass(3.0f);
        L_CHECK(rbData.getMass() == 3.0f);
           
        rbData.setInertiaTensor(math::mat3(5.0f));
        L_CHECK(rbData.getInertiaTensor() == math::mat3(5.0f));

        LEGION_SUBTEST("Rigidbody Event Generation: Flagged and Unflagged")
        {
            auto& modifyEvents = rbData.getGeneratedModifyEvents();

            L_CHECK(modifyEvents.test(physics::rigidbody_flag::rb_mass));

            L_CHECK(modifyEvents.test(physics::rigidbody_flag::rb_inertia_tensor));
            
            L_CHECK( NOT modifyEvents.test(physics::rigidbody_flag::rb_velocity));

            L_CHECK( NOT modifyEvents.test(physics::rigidbody_flag::rb_angular_velocity));

            L_CHECK( NOT modifyEvents.test(physics::rigidbody_flag::rb_linear_drag));

            L_CHECK( NOT modifyEvents.test(physics::rigidbody_flag::rb_angular_drag));
        }

        rbData.resetModificationFlags();

        rbData.setVelocity(math::vec3(5.0f));
        L_CHECK(rbData.getVelocity() == math::vec3(5.0f));

        rbData.setAngularVelocity(math::vec3(5.0f));
        L_CHECK(rbData.getAngularVelocity() == math::vec3(5.0f));

        rbData.setLinearDrag(0.02f);
        L_CHECK(rbData.getLinearDrag() == 0.02f);

        rbData.setAngularDrag(0.05f);
        L_CHECK(rbData.getAngularDrag() == 0.05f);

        LEGION_SUBTEST("Rigidbody Event Generation: Modification Reset")
        {
            auto& modifyEvents = rbData.getGeneratedModifyEvents();

            L_CHECK( NOT modifyEvents.test(physics::rigidbody_flag::rb_mass));

            L_CHECK( NOT modifyEvents.test(physics::rigidbody_flag::rb_inertia_tensor));

            L_CHECK(modifyEvents.test(physics::rigidbody_flag::rb_velocity));

            L_CHECK(modifyEvents.test(physics::rigidbody_flag::rb_angular_velocity));

            L_CHECK(modifyEvents.test(physics::rigidbody_flag::rb_linear_drag));

            L_CHECK(modifyEvents.test(physics::rigidbody_flag::rb_angular_drag));
        }
    }
}

LEGION_TEST("physics::rigidbody")
{
    Test(TestRigidbody);
}
