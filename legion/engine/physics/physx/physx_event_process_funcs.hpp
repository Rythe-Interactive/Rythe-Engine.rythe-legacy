#pragma once
#include <core/core.hpp>
#include <physics/physx/data/physx_wrapper.hpp>

namespace physx
{
    class PxScene;
}

namespace legion::physics
{
    void processAddBoxEvent(events::event_base* addBoxEvent, physx::PxScene* scene, PhysxInternalWrapper& wrapper, ecs::entity entity);
}
