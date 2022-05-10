#pragma once
#include <core/core.hpp>

namespace physx
{
    class PxActor;
    class PxController;
}

namespace legion::physics
{
    class ControllerHitFeedback;

    enum class physics_body_type
    {
        none,
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

    struct PhysxCharacterWrapper
    {
        physx::PxController* characterController = nullptr;
        std::unique_ptr<ControllerHitFeedback> controllerFeedback;
        math::vec3 totalDisplacement = math::vec3(0.0f);

        PhysxCharacterWrapper();
        ~PhysxCharacterWrapper();

        PhysxCharacterWrapper& operator= (PhysxCharacterWrapper& other) = delete;

        PhysxCharacterWrapper(PhysxCharacterWrapper& other) = delete;

        PhysxCharacterWrapper& operator= (PhysxCharacterWrapper&& other) noexcept;

        PhysxCharacterWrapper(PhysxCharacterWrapper&& other) noexcept;
    };
}
