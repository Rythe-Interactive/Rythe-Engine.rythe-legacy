#pragma once
#include <core/core.hpp>
#include <physics/physx/data/physx_wrapper.hpp>

namespace physx
{
    class PxScene;
}

namespace legion::physics
{
    void processAddBoxEvent(events::event_base* addBoxEvent, PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity);
}
