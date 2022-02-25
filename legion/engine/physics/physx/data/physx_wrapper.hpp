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
        static_collider,
        rigidbody
    };

    struct PhysxInternalWrapper
    {
        physx::PxActor* physicsActor = nullptr;

        physics_body_type bodyType = physics_body_type::static_collider;

        PhysxInternalWrapper() noexcept = default;

        ~PhysxInternalWrapper();

        PhysxInternalWrapper(PhysxInternalWrapper&& other) noexcept;

        PhysxInternalWrapper& operator= (PhysxInternalWrapper&& other) noexcept;

        PhysxInternalWrapper& operator= (const PhysxInternalWrapper& other) = delete;

        PhysxInternalWrapper(const PhysxInternalWrapper& other) = delete;
    };
}
