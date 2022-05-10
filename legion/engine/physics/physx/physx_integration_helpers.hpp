#pragma once
#include <core/core.hpp>
#include <physics/data/component_flags.hpp>

namespace physx
{
    class PxMaterial;
    class PxScene;
    class PxPhysics;
    class PxTransform;
    class PxShape;
};

namespace legion::physics
{
    class ColliderData;
    class CollisionFilter;
    class PhysxInternalWrapper;

    struct PhysxEnviromentInfo
    {
        physx::PxScene* scene = nullptr;
        physx::PxMaterial* defaultMaterial = nullptr;
        float timeStep = 0.02f;
        std::unordered_map<size_type, physx::PxMaterial*>* physicsMaterials;
        float defaultRigidbodyDensity = 1.0f;
    };

    physx::PxPhysics* getSDK();

    void setShapeFilterData(physx::PxShape* shape, const CollisionFilter& collisionFilter, physics_object_flag objectType);

    void calculateLocalColliderTransform(physx::PxTransform& outLocalTransform, const ColliderData& collider);

    void calculateGlobalAndLocalTransforms(
        physx::PxTransform& outLocalTransform, physx::PxTransform& outGlobalTransform, const ColliderData& collider,ecs::entity ent);

    template<class PxGeometry,class... GeometryArgs>
    void instantiateStaticActorWith(physx::PxPhysics* sdk, PhysxInternalWrapper& wrapper, const  ColliderData& collider,
        const physx::PxTransform& globalTransform, const physx::PxTransform& localTransform, const PhysxEnviromentInfo& sceneInfo, ecs::entity ent,  GeometryArgs&&... geometryArgs);

    template<class PxGeometry, class... GeometryArgs>
    void instantiateDynamicActorWith(physx::PxPhysics* sdk, PhysxInternalWrapper& wrapper, const  ColliderData& collider,
        const physx::PxTransform& globalTransform, const physx::PxTransform& localTransform, const PhysxEnviromentInfo& sceneInfo, ecs::entity ent, GeometryArgs&&... geometryArgs);

    template<class PxGeometry, class... GeometryArgs>
    void instantiateNextCollider(physx::PxPhysics* sdk, PhysxInternalWrapper& wrapper, const ColliderData& collider,
        const physx::PxTransform& localTransform, const PhysxEnviromentInfo& sceneInfo, GeometryArgs&&... geometryArgs);
}
