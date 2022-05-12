#include "physx_wrapper.hpp"
#include <physx/PxPhysicsAPI.h>
#include <physics/physx/data/controller_hit_feedback.inl>

namespace legion::physics
{
    PhysxInternalWrapper::~PhysxInternalWrapper()
    {
        if (physicsActor)
        {
            physicsActor->release();
            physicsActor = nullptr;
        }
    }

    PhysxInternalWrapper::PhysxInternalWrapper(PhysxInternalWrapper&& other) noexcept
        : bodyType(other.bodyType), physicsActor(other.physicsActor)
    {
        other.bodyType = physics_body_type::none;
        other.physicsActor = nullptr;
    }

    PhysxInternalWrapper& PhysxInternalWrapper::operator=(PhysxInternalWrapper&& other) noexcept
    {
        bodyType = other.bodyType;
        physicsActor = other.physicsActor;

        other.bodyType = physics_body_type::none;
        other.physicsActor = nullptr;

        return *this;
    }

    PhysxCharacterWrapper::PhysxCharacterWrapper() { }

    PhysxCharacterWrapper::~PhysxCharacterWrapper() { }

    PhysxCharacterWrapper& PhysxCharacterWrapper::operator= (PhysxCharacterWrapper&& other) noexcept
    {
        characterController = other.characterController;
        controllerFeedback = std::move(other.controllerFeedback);
        totalDisplacement = other.totalDisplacement;
        actorsCollidedWith = std::move(other.actorsCollidedWith);

        other.characterController = nullptr;

        return *this;
    }

    PhysxCharacterWrapper::PhysxCharacterWrapper(PhysxCharacterWrapper&& other) noexcept
        : characterController(other.characterController), controllerFeedback(std::move(other.controllerFeedback)),
        totalDisplacement(other.totalDisplacement), actorsCollidedWith(std::move(other.actorsCollidedWith))
    {
        other.characterController = nullptr;
    }
}
