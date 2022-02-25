#pragma once

namespace physx
{
    class PxScene;
};

namespace legion::physics
{
    struct PhysxEnviroment
    {
        physx::PxScene* scene = nullptr;

    };


}
