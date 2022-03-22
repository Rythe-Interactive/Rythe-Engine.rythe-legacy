#include "physx_wrapper.hpp"
#include <physx/PxPhysicsAPI.h>

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
}
