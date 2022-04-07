#pragma once
#include <core/core.hpp>

namespace physx
{
    class PxMaterial;
    class PxScene;
    class PxPhysics;
    class PxTransform;
};

namespace legion::physics
{
    class ColliderData;
    class PhysxInternalWrapper;

    struct PhysxEnviromentInfo
    {
        physx::PxScene* scene = nullptr;
        physx::PxMaterial* defaultMaterial = nullptr;
        std::unordered_map<size_type, physx::PxMaterial*>* physicsMaterials;
        float defaultRigidbodyDensity = 1.0f;
    };

    physx::PxPhysics* getSDK();

    void calculateLocalColliderTransform(physx::PxTransform& outLocalTransform, const ColliderData& collider);

    void calculateGlobalAndLocalTransforms(
        physx::PxTransform& outLocalTransform, physx::PxTransform& outGlobalTransform, const ColliderData& collider,ecs::entity ent);

    template<class PxGeometry,class... GeometryArgs>
    void instantiateStaticActorWith(physx::PxPhysics* sdk, PhysxInternalWrapper& wrapper, const physx::PxTransform& globalTransform,
        const physx::PxTransform& localTransform, const PhysxEnviromentInfo& sceneInfo, ecs::entity ent,  GeometryArgs&&... geometryArgs);

    template<class PxGeometry, class... GeometryArgs>
    void instantiateDynamicActorWith(physx::PxPhysics* sdk, PhysxInternalWrapper& wrapper, const physx::PxTransform& globalTransform,
        const physx::PxTransform& localTransform, const PhysxEnviromentInfo& sceneInfo, ecs::entity ent, GeometryArgs&&... geometryArgs);

    template<class PxGeometry, class... GeometryArgs>
    void instantiateNextCollider(physx::PxPhysics* sdk, PhysxInternalWrapper& wrapper,
        const physx::PxTransform& localTransform, const PhysxEnviromentInfo& sceneInfo, GeometryArgs&&... geometryArgs);
}
