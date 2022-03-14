#include "physx_integration_helpers.hpp"
#include <physics/physx/systems/physx_physics_system.hpp>
#include <physx/PxPhysicsAPI.h>

using namespace physx;

namespace legion::physics
{
    physx::PxPhysics* getSDK()
    {
        return PhysXPhysicsSystem::getSDK();
    }

    void calculateGlobalAndLocalTransforms(physx::PxTransform& outLocalTransform, physx::PxTransform& outGlobalTransform, const ColliderData& collider, ecs::entity ent)
    {
        const math::vec3& pos = *ent.get_component<position>();
        const math::quat& rot = *ent.get_component<rotation>();

        const math::vec3& localOffset = collider.getOffset();
        const math::quat& localRot = collider.getRotationOffset();

        toPhysxTransform(outGlobalTransform, pos, rot);

        toPhysxTransform(outLocalTransform, localOffset, localRot);
    }

    inline void toPhysxTransform(physx::PxTransform& pxTransform, const math::vec3& pos, const math::quat& rot)
    {
        pxTransform.p = { pos.x,pos.y,pos.z };
        pxTransform.q = { rot.x,rot.y,rot.z,rot.w };
    }

    template<class PxGeometry, class ...GeometryArgs>
    void instantiateStaticActorWith(physx::PxPhysics* sdk, PhysxInternalWrapper& wrapper,
        const physx::PxTransform& globalTransform, const physx::PxTransform& localTransform, const PhysxEnviromentInfo& sceneInfo, ecs::entity ent, GeometryArgs... geometryArgs)
    {
        PxRigidStatic* staticCollider = PxCreateStatic(*getSDK(), globalTransform, 
            PxGeometry(std::forward<GeometryArgs>(geometryArgs)...), *sceneInfo.defaultMaterial, localTransform);
        staticCollider->userData = ent.data;

        wrapper.physicsActor = staticCollider;
        sceneInfo.scene->addActor(*staticCollider);
    }

    template<class PxGeometry, class ...GeometryArgs>
    void instantiateDynamicActorWith(physx::PxPhysics* sdk, PhysxInternalWrapper& wrapper,
        const physx::PxTransform& globalTransform, const physx::PxTransform& localTransform, const PhysxEnviromentInfo& sceneInfo, ecs::entity ent, GeometryArgs... geometryArgs)
    {
        PxRigidDynamic* rb = PxCreateDynamic(*getSDK(), globalTransform,
            PxGeometry(std::forward<GeometryArgs>(geometryArgs)...), *sceneInfo.defaultMaterial, 1.0f, localTransform);
        rb->userData = ent.data;

        wrapper.physicsActor = rb;
        sceneInfo.scene->addActor(*rb);
    }


    template<class PxGeometry, class ...GeometryArgs>
    void instantiateNextCollider(physx::PxPhysics* sdk, PhysxInternalWrapper& wrapper, const physx::PxTransform& globalTransform, const physx::PxTransform& localTransform,
        const PhysxEnviromentInfo& sceneInfo, GeometryArgs ...geometryArgs)
    {
        PxShape* shape = getSDK()->createShape(PxGeometry(std::forward<GeometryArgs>(geometryArgs)...), *sceneInfo.defaultMaterial, true);

        PxRigidActor* rigid = static_cast<PxRigidActor*>(wrapper.physicsActor);

        shape->setLocalPose(localTransform);
        rigid->attachShape(*shape);
        shape->release();
    }

    //static collider instantiations
    template void instantiateStaticActorWith<PxSphereGeometry, float>(physx::PxPhysics* sdk, PhysxInternalWrapper& wrapper,
        const physx::PxTransform& outGlobalTransform, const physx::PxTransform& outLocalTransform, const PhysxEnviromentInfo& sceneInfo, ecs::entity ent, float x);

    template void instantiateStaticActorWith<PxBoxGeometry, const PxVec3&>(physx::PxPhysics* sdk, PhysxInternalWrapper& wrapper,
        const physx::PxTransform& outGlobalTransform, const physx::PxTransform& outLocalTransform, const PhysxEnviromentInfo& sceneInfo, ecs::entity ent, const PxVec3& extents);

    //dynamic collider instantiations
    template void instantiateDynamicActorWith<PxSphereGeometry, float>(physx::PxPhysics* sdk, PhysxInternalWrapper& wrapper,
        const physx::PxTransform& outGlobalTransform, const physx::PxTransform& outLocalTransform, const PhysxEnviromentInfo& sceneInfo, ecs::entity ent, float x);

    template void instantiateDynamicActorWith<PxBoxGeometry, const PxVec3&>(physx::PxPhysics* sdk, PhysxInternalWrapper& wrapper,
        const physx::PxTransform& outGlobalTransform, const physx::PxTransform& outLocalTransform, const PhysxEnviromentInfo& sceneInfo, ecs::entity ent, const PxVec3& extents);

    //next collider instantiations
    template void instantiateNextCollider<PxBoxGeometry, const PxVec3&>(physx::PxPhysics* sdk, PhysxInternalWrapper& wrapper,
        const physx::PxTransform& outGlobalTransform, const physx::PxTransform& outLocalTransform,const PhysxEnviromentInfo& sceneInfo, const PxVec3& extents);

    template void instantiateNextCollider<PxSphereGeometry, float>(physx::PxPhysics* sdk, PhysxInternalWrapper& wrapper,
        const physx::PxTransform& outGlobalTransform, const physx::PxTransform& outLocalTransform, const PhysxEnviromentInfo& sceneInfo, float x);
}
