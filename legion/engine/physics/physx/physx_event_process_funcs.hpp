#pragma once
#include <core/core.hpp>
#include <physics/physx/data/physx_wrapper.hpp>
#include <physics/physx/physx_integration_helpers.hpp>
#include <physics/components/physics_component.hpp>
#include <physics/components/physics_enviroment.hpp>


namespace physx
{
    class PxScene;
}

namespace legion::physics
{
    class rigidbody;

    //----------------------------------------------------------- Rigidbody -------------------------------------------------------------------------//

    void processAngularDragModification(rigidbody& rigidbody, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity);

    void processLinearDragModification(rigidbody& rigidbody, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity);

    void processVelocityModification(rigidbody& rigidbody,const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity);

    void processAngularVelocityModification(rigidbody& rigidbody, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity);

    void processMassModification(rigidbody& rigidbody, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity);

    //----------------------------------------------------------- Physics Component -------------------------------------------------------------------------//

    void processAddFirstBox(physics_component& physicsComponent, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity);

    void processAddNextBox(physics_component& physicsComponent, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity);

    void processAddFirstSphere(physics_component& physicsComponent, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity);

    void processAddNextSphere(physics_component& physicsComponent, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity);

    void processAddFirstConvex(physics_component& physicsComponent, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity);

    void processAddNextConvex(physics_component& physicsComponent, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity);

    //----------------------------------------------------------- Physics Enviroment -------------------------------------------------------------------------//

    void processAddInfinitePlane(physics_enviroment& physicsEnviroment, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity);
}
