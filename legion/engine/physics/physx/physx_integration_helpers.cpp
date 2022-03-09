#include "physx_integration_helpers.hpp"
#include <physics/physx/systems/physx_physics_system.hpp>
#include <physx/PxPhysicsAPI.h>

using namespace physx;

namespace legion::physics
{
    physx::PxPhysics* legion::physics::getSDK()
    {
        return PhysXPhysicsSystem::getSDK();
    }

    inline void toPhysxTransform(physx::PxTransform& pxTransform, math::vec3& pos, math::quat& rot)
    {
        pxTransform.p = { pos.x,pos.y,pos.z };
        pxTransform.q = { rot.x,rot.y,rot.z,rot.w };
    }
}
