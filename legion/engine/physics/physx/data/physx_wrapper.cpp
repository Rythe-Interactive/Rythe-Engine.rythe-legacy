#include "physx_wrapper.hpp"
#include <physx/PxPhysicsAPI.h>

namespace legion::physics
{
    PhysxInternalWrapper::~PhysxInternalWrapper()
    {
        for (physx::PxActor* actor : physicsActors)
        {
            actor->release();
        }

        physicsActors.clear();
    }

    PhysxInternalWrapper::PhysxInternalWrapper(PhysxInternalWrapper&& other) noexcept
        : bodyType(other.bodyType), physicsActors(std::move(other.physicsActors))
    {
        other.bodyType = physics_body_type::none;
    }

    PhysxInternalWrapper& PhysxInternalWrapper::operator=(PhysxInternalWrapper&& other) noexcept
    {
        bodyType = other.bodyType;
        physicsActors = std::move(other.physicsActors);

        other.bodyType = physics_body_type::none;

        return *this;
    }
}
