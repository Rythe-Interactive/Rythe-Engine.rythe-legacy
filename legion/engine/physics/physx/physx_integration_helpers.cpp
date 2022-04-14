#include "physx_integration_helpers.hpp"
#include <physics/physx/data/physx_wrapper.hpp>
#include <physics/physx/systems/physx_physics_system.hpp>
#include <physx/PxPhysicsAPI.h>

using namespace physx;

namespace legion::physics
{
    physx::PxPhysics* getSDK()
    {
        return PhysXPhysicsSystem::getSDK();
    }

    static inline void toPhysxTransform(physx::PxTransform& pxTransform, const math::vec3& pos, const math::quat& rot)
    {
        pxTransform.p = { pos.x,pos.y,pos.z };
        pxTransform.q = { rot.x,rot.y,rot.z,rot.w };
    }

    void calculateLocalColliderTransform(physx::PxTransform& outLocalTransform, const ColliderData& collider)
    {
        const math::vec3& localOffset = collider.getOffset();
        const math::quat& localRot = collider.getRotationOffset();

        toPhysxTransform(outLocalTransform, localOffset, localRot);
    }

    void calculateGlobalAndLocalTransforms(physx::PxTransform& outLocalTransform, physx::PxTransform& outGlobalTransform, const ColliderData& collider, ecs::entity ent)
    {
        transform trans = ent.get_component<transform>();
        const math::mat4& globalTransform = trans.to_world_matrix();

        math::vec3 pos; math::quat rot; math::vec3 tempScale;
        math::decompose(globalTransform, tempScale, rot, pos);
       
        calculateLocalColliderTransform(outLocalTransform, collider);
        toPhysxTransform(outGlobalTransform, pos, rot);
    }

    template<class PxGeometry, class ...GeometryArgs>
    void instantiateStaticActorWith(physx::PxPhysics* sdk, PhysxInternalWrapper& wrapper,
        const physx::PxTransform& globalTransform, const physx::PxTransform& localTransform, const PhysxEnviromentInfo& sceneInfo, ecs::entity ent, GeometryArgs&&... geometryArgs)
    {
        PxRigidStatic* staticCollider = PxCreateStatic(*getSDK(), globalTransform, 
            PxGeometry(std::forward<GeometryArgs>(geometryArgs)...), *sceneInfo.defaultMaterial, localTransform);
        staticCollider->userData = ent.data;

        wrapper.physicsActor = staticCollider;
        sceneInfo.scene->addActor(*staticCollider);
    }

    template<class PxGeometry, class ...GeometryArgs>
    void instantiateDynamicActorWith(physx::PxPhysics* sdk, PhysxInternalWrapper& wrapper,
        const physx::PxTransform& globalTransform, const physx::PxTransform& localTransform, const PhysxEnviromentInfo& sceneInfo, ecs::entity ent, GeometryArgs&&... geometryArgs)
    {
        PxRigidDynamic* dynamic = PxCreateDynamic(*getSDK(), globalTransform,
            PxGeometry(std::forward<GeometryArgs>(geometryArgs)...), *sceneInfo.defaultMaterial, sceneInfo.defaultRigidbodyDensity, localTransform);
        dynamic->userData = ent.data;
        auto& rbData = ( * ent.get_component<rigidbody>() ).data;

        rbData.setDensityDirect(sceneInfo.defaultRigidbodyDensity);

        if (! rbData.getGeneratedModifyEvents().test(rigidbody_flag::rb_mass))
        {
            rbData.setMassDirect(dynamic->getMass());
        }
       
        wrapper.physicsActor = dynamic;
        sceneInfo.scene->addActor(*dynamic);
    }


    template<class PxGeometry, class ...GeometryArgs>
    void instantiateNextCollider(physx::PxPhysics* sdk, PhysxInternalWrapper& wrapper, const physx::PxTransform& localTransform,
        const PhysxEnviromentInfo& sceneInfo, GeometryArgs&& ...geometryArgs)
    {
        PxShape* shape = getSDK()->createShape(PxGeometry(std::forward<GeometryArgs>(geometryArgs)...), *sceneInfo.defaultMaterial, true);

        PxRigidActor* rigid = static_cast<PxRigidActor*>(wrapper.physicsActor);

        shape->setLocalPose(localTransform);
        rigid->attachShape(*shape);
        shape->release();
    }

#define DECLARE_STATIC_COLLIDER_TEMPLATE_INSTANTIATION(PxGeometry,...)  template void instantiateStaticActorWith<PxGeometry, __VA_ARGS__>(physx::PxPhysics* sdk, PhysxInternalWrapper& wrapper, const physx::PxTransform& outGlobalTransform, const physx::PxTransform& outLocalTransform, const PhysxEnviromentInfo& sceneInfo, ecs::entity ent, __VA_ARGS__);

    DECLARE_STATIC_COLLIDER_TEMPLATE_INSTANTIATION(PxSphereGeometry, float&);
    DECLARE_STATIC_COLLIDER_TEMPLATE_INSTANTIATION(PxBoxGeometry, const PxVec3&);
    DECLARE_STATIC_COLLIDER_TEMPLATE_INSTANTIATION(PxConvexMeshGeometry, PxConvexMesh*&);

#define DECLARE_DYNAMIC_COLLIDER_TEMPLATE_INSTANTIATION(PxGeometry,...) template void instantiateDynamicActorWith<PxGeometry, __VA_ARGS__>(physx::PxPhysics* sdk, PhysxInternalWrapper& wrapper,const physx::PxTransform& outGlobalTransform, const physx::PxTransform& outLocalTransform, const PhysxEnviromentInfo& sceneInfo, ecs::entity ent, __VA_ARGS__);

    DECLARE_DYNAMIC_COLLIDER_TEMPLATE_INSTANTIATION(PxSphereGeometry, float&);
    DECLARE_DYNAMIC_COLLIDER_TEMPLATE_INSTANTIATION(PxBoxGeometry, const PxVec3&);
    DECLARE_DYNAMIC_COLLIDER_TEMPLATE_INSTANTIATION(PxConvexMeshGeometry, PxConvexMesh*&);

#define DECLARE_NEXT_COLLIDER_TEMPLATE_INSTANTIATION(PxGeometry,...) template void instantiateNextCollider<PxGeometry, __VA_ARGS__>(physx::PxPhysics* sdk, PhysxInternalWrapper& wrapper, const physx::PxTransform& outLocalTransform, const PhysxEnviromentInfo& sceneInfo, __VA_ARGS__);

    DECLARE_NEXT_COLLIDER_TEMPLATE_INSTANTIATION(PxSphereGeometry, float&);
    DECLARE_NEXT_COLLIDER_TEMPLATE_INSTANTIATION(PxBoxGeometry, const PxVec3&);
    DECLARE_NEXT_COLLIDER_TEMPLATE_INSTANTIATION(PxConvexMeshGeometry, PxConvexMesh*&);
}
