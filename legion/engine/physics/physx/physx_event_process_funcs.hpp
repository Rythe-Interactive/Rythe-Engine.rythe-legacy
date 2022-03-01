#pragma once
#include <core/core.hpp>
#include <physics/physx/data/physx_wrapper.hpp>
#include <physics/physx/physx_integration_helpers.hpp>

namespace physx
{
    class PxScene;
}

namespace legion::physics
{
    void processAddBoxEvent(events::event_base* addBoxEvent, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity);
}
