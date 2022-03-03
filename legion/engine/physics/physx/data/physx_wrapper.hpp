#pragma once
#include <core/core.hpp>

namespace physx
{
    class PxActor;
}

namespace legion::physics
{
    enum class physics_body_type
    {
        none,
        static_collider,
        rigidbody
    };

    struct PhysxInternalWrapper
    {
        std::vector<physx::PxActor*> physicsActors;

        physics_body_type bodyType = physics_body_type::static_collider;

        PhysxInternalWrapper() = default;

        ~PhysxInternalWrapper();

        PhysxInternalWrapper(PhysxInternalWrapper&& other) noexcept;

        PhysxInternalWrapper& operator= (PhysxInternalWrapper&& other) noexcept;

        PhysxInternalWrapper& operator= (const PhysxInternalWrapper&& other) = delete;

        PhysxInternalWrapper(const PhysxInternalWrapper& other) = delete;
    };
}
