#pragma once
#include <core/core.hpp>
#include <physics/physx/data/physx_wrapper.hpp>
#include <physics/physx/physx_integration_helpers.hpp>
#include <physics/components/physics_component.hpp>

namespace physx
{
    class PxScene;
}

namespace legion::physics
{
    void processVelocityChange(const core::events::event_base& velocityChangeEvent, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity);

    void processAddBoxEvent(physicsComponent& physicsComponent, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity);
}
