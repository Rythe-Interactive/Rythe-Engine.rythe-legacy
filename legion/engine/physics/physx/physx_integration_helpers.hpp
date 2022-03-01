#pragma once

namespace physx
{
    class PxMaterial;
    class PxScene;
    class PxPhysics;
};

namespace legion::physics
{
    struct PhysxEnviromentInfo
    {
        physx::PxScene* scene = nullptr;
        physx::PxMaterial* defaultMaterial = nullptr;
    };

    physx::PxPhysics* getSDK();

}
