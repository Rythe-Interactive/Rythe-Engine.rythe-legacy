#pragma once
#include <core/core.hpp>

namespace physx
{
    class PxMaterial;
    class PxScene;
    class PxPhysics;
    class PxTransform;
};

namespace legion::physics
{
    struct PhysxEnviromentInfo
    {
        physx::PxScene* scene = nullptr;
        physx::PxMaterial* defaultMaterial = nullptr;
    };

    physx::PxPhysics* getSDK();

    inline void toPhysxTransform(physx::PxTransform& pxTransform, math::vec3& pos, math::quat& rot);
}
