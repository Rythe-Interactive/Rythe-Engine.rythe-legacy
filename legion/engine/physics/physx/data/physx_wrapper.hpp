#pragma once
#include <core/core.hpp>

namespace physx
{
    class PxActor;
}

namespace legion::physics
{
    constexpr size_type invalidPhysXWrapperID = std::numeric_limits<size_type>::max();

    enum class physics_body_type
    {
        static_collider,
        rigidbody
    };

    struct PhysxInternalWrapper
    {
        size_type id = invalidPhysXWrapperID;
       
        std::vector<physx::PxActor*> physicsActors;

        physics_body_type bodyType = physics_body_type::static_collider;
    };
}
