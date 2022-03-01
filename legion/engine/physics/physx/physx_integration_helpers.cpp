#include "physx_integration_helpers.hpp"
#include <physics/physx/systems/physx_physics_system.hpp>
#include <physx/PxPhysicsAPI.h>

namespace legion::physics
{
    physx::PxPhysics* legion::physics::getSDK()
    {
        return PhysXPhysicsSystem::getSDK();
    }
}
