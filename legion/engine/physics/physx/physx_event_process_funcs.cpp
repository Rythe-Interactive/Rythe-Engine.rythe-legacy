#include <physics/physx/physx_event_process_funcs.hpp>
#include <physx/PxPhysicsAPI.h>
#include <physics/physx/physx_integration_helpers.hpp>
#include <physics/events/events.hpp>
#include <physics/components/physics_component.hpp>
#include <physics/components/rigidbody.hpp>
#include <physics/physx/physx_event_process_funcs.hpp>

using namespace physx;

namespace legion::physics
{
    void processAngularDragModification(rigidbody& rigidbody, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity)
    {
        float angularDrag = rigidbody.data.getAngularDrag();
        PxRigidDynamic* rigid = static_cast<PxRigidDynamic*>(wrapper.physicsActor);

        rigid->setAngularDamping(angularDrag);
    }

    void processLinearDragModification(rigidbody& rigidbody, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity)
    {
        float linearDrag = rigidbody.data.getLinearDrag();
        PxRigidDynamic* rigid = static_cast<PxRigidDynamic*>(wrapper.physicsActor);

        rigid->setLinearDamping(linearDrag);
    }

    void processVelocityModification(rigidbody& rigidbody, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity)
    {
        const math::vec3& vel = rigidbody.data.getVelocity();

        PxRigidDynamic* rigid = static_cast<PxRigidDynamic*>(wrapper.physicsActor);
        
        PxVec3 pxVelocity(vel.x, vel.y, vel.z);
        rigid->setLinearVelocity(pxVelocity);
    }

    void processAngularVelocityModification(rigidbody& rigidbody, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity)
    {
        const math::vec3& angularVel = rigidbody.data.getAngularVelocity();

        PxRigidDynamic* rigid = static_cast<PxRigidDynamic*>(wrapper.physicsActor);

        PxVec3 pxAng(angularVel.x, angularVel.y, angularVel.z);
        rigid->setAngularVelocity(pxAng);
    }

    void processMassModification(rigidbody& rigidbody, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity)
    {
        float newMass = rigidbody.data.getMass();
        float density = rigidbody.data.getDensity();

        PxRigidDynamic* rigid = static_cast<PxRigidDynamic*>(wrapper.physicsActor);
        float oldMass = rigid->getMass();

        float newDensity = density * (newMass / oldMass);
        PxRigidBodyExt::updateMassAndInertia(*rigid, newDensity);

        float finalMass = rigid->getMass();
        rigidbody.data.setDensityDirect(newDensity);
    }

    void processAddNextBox(physics_component& physicsComponent, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity)
    {
        PhysicsComponentData& data = physicsComponent.physicsCompData;

        for (ColliderData& collider : data.getColliders())
        {
            if (collider.isRegisteredOrNotOfType(collider_type::box)) { continue; }
            collider.setRegistered(true);

            PxTransform localTransform;
            calculateLocalColliderTransform(localTransform, collider);

            const math::vec3& extents = *collider.getBoxExtents();

            instantiateNextCollider<PxBoxGeometry,const PxVec3&>(getSDK(), wrapper, localTransform, sceneInfo, PxVec3(extents.x, extents.y, extents.z));
        }
    }
    
    void processAddFirstBox(physics_component& physicsComponent, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity)
    {
        PhysicsComponentData& data = physicsComponent.physicsCompData;

        for (ColliderData& collider : data.getColliders())
        {
            if (collider.isRegisteredOrNotOfType(collider_type::box)) { continue; }
            collider.setRegistered(true);
            
            const math::vec3& extents = *collider.getBoxExtents();

            PxTransform transform;
            PxTransform localTransform;

            calculateGlobalAndLocalTransforms(localTransform, transform, collider, entity);

            switch (wrapper.bodyType)
            {
            case physics_body_type::rigidbody:
                instantiateDynamicActorWith<PxBoxGeometry, const PxVec3&>(getSDK(), wrapper, transform,
                    localTransform, sceneInfo, entity, PxVec3(extents.x, extents.y, extents.z));
                break;
            case physics_body_type::static_collider:
                instantiateStaticActorWith<PxBoxGeometry, const PxVec3&>(getSDK(), wrapper, transform,
                    localTransform, sceneInfo, entity, PxVec3(extents.x, extents.y, extents.z));
                break;
            default:
                log::warn("PhysxInternalWrapper::bodyType set to an unsupported body type");
            }
  
            break;
        }
    }

    void processAddFirstSphere(physics_component& physicsComponent, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity)
    {
        PhysicsComponentData& data = physicsComponent.physicsCompData;

        for (ColliderData& collider : data.getColliders())
        {
            if (collider.isRegisteredOrNotOfType(collider_type::sphere)) { continue; }
            collider.setRegistered(true);

            PxTransform transform;
            PxTransform localTransform;

            calculateGlobalAndLocalTransforms(localTransform, transform, collider, entity);

            float radius = *collider.getSphereRadius();

            switch (wrapper.bodyType)
            {
            case physics_body_type::rigidbody:
                instantiateDynamicActorWith<PxSphereGeometry, float&>(getSDK(), wrapper, transform, localTransform, sceneInfo, entity, radius);
                break;
            case physics_body_type::static_collider:
                instantiateStaticActorWith<PxSphereGeometry, float&>(getSDK(), wrapper, transform, localTransform, sceneInfo, entity, radius);
                break;
            default:
                log::warn("PhysxInternalWrapper::bodyType set to an unsupported body type");
            }

            break;
        }
    }

    void processAddNextSphere(physics_component& physicsComponent, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity)
    {
        PhysicsComponentData& data = physicsComponent.physicsCompData;

        for (ColliderData& collider : data.getColliders())
        {
            if (collider.isRegisteredOrNotOfType(collider_type::sphere)) { continue; }
            collider.setRegistered(true);

            PxTransform localTransform;
            calculateLocalColliderTransform(localTransform, collider);

            float radius = *collider.getSphereRadius();

            instantiateNextCollider<PxSphereGeometry, float&>(getSDK(), wrapper, localTransform, sceneInfo, radius);
        }
    }

    void processAddFirstConvex(physics_component& physicsComponent, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity)
    {
        PhysicsComponentData& data = physicsComponent.physicsCompData;

        for (ColliderData& collider : data.getColliders())
        {
            if (collider.isRegisteredOrNotOfType(collider_type::quickhull_convex)) { continue; }
            collider.setRegistered(true);

            PxTransform transform;
            PxTransform localTransform;

            calculateGlobalAndLocalTransforms(localTransform, transform, collider, entity);

            PxConvexMesh* convexMesh = static_cast<PxConvexMesh*>(collider.getConvexCollider());

            switch (wrapper.bodyType)
            {
            case physics_body_type::rigidbody:
                instantiateDynamicActorWith<PxConvexMeshGeometry, PxConvexMesh*&>(getSDK(), wrapper, transform, localTransform, sceneInfo, entity, convexMesh);
                break;
            case physics_body_type::static_collider:
                instantiateStaticActorWith<PxConvexMeshGeometry, PxConvexMesh*&>(getSDK(), wrapper, transform, localTransform, sceneInfo, entity, convexMesh);
                break;
            default:
                log::warn("PhysxInternalWrapper::bodyType set to an unsupported body type");
            }
        }
    }

    void processAddNextConvex(physics_component& physicsComponent, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity)
    {
        PhysicsComponentData& data = physicsComponent.physicsCompData;

        for (ColliderData& collider : data.getColliders())
        {
            if (!collider.isRegisteredOrNotOfType(collider_type::quickhull_convex)) { continue; }
            collider.setRegistered(true);

            PxTransform localTransform;
            calculateLocalColliderTransform(localTransform, collider);

            PxConvexMesh* convexMesh = static_cast<PxConvexMesh*>(collider.getConvexCollider());
            instantiateNextCollider<PxConvexMeshGeometry, PxConvexMesh*&>(getSDK(), wrapper, localTransform, sceneInfo, convexMesh);
        }
    }

    void processSetPhysicsMaterial(const ColliderData& collider, const collider_modification_data& modData, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper)
    {
        PxRigidActor* rigid = static_cast<PxRigidActor*>(wrapper.physicsActor);

        PxU32 shapeCount = rigid->getNbShapes();
        PxShape* shapes = static_cast<PxShape *>(_alloca(sizeof(PxShape*) * shapeCount));

        rigid->getShapes(&shapes, shapeCount);

        PxMaterial* newMaterial = sceneInfo.defaultMaterial;

        size_type materialHash = modData.data.newMaterial;
        auto iter = sceneInfo.physicsMaterials->find(materialHash);

        if (iter != sceneInfo.physicsMaterials->end())
        {
            newMaterial = iter->second;
        }
        else
        {
            log::warn("Failed to assign physics material!, default material has been set");
        }

        shapes[collider.getColliderIndex()].setMaterials(&newMaterial, 1);
    }

    void processSetBoxSize(const ColliderData& collider, const collider_modification_data& modData, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper)
    {
        PxRigidActor* rigid = static_cast<PxRigidActor*>(wrapper.physicsActor);

        PxU32 shapeCount = rigid->getNbShapes();
        PxShape* shapes = static_cast<PxShape*>(_alloca(sizeof(PxShape*) * shapeCount));
        rigid->getShapes(&shapes, shapeCount);

        const math::vec3& newExtents = modData.data.newBoxExtents;

        PxBoxGeometry box;
        shapes[modData.colliderIndex].getBoxGeometry(box);

        box.halfExtents = { newExtents.x,newExtents.y,newExtents.z };

        shapes[modData.colliderIndex].setGeometry(box);
    }

    void processSetSphereSize(const ColliderData& collider, const collider_modification_data& modData, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper)
    {
        PxRigidActor* rigid = static_cast<PxRigidActor*>(wrapper.physicsActor);

        PxU32 shapeCount = rigid->getNbShapes();
        PxShape* shapes = static_cast<PxShape*>(_alloca(sizeof(PxShape*) * shapeCount));
        rigid->getShapes(&shapes, shapeCount);

        float newRadius = modData.data.newRadius;

        PxSphereGeometry sphere;
        shapes[modData.colliderIndex].getSphereGeometry(sphere);

        sphere.radius = newRadius;

        shapes[modData.colliderIndex].setGeometry(sphere);
    }

    void processAddInfinitePlane(physics_enviroment& physicsEnviroment, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity)
    {
        const math::vec3& normal = physicsEnviroment.data.getInfinitePlaneNormal();
        float distToPlane = physicsEnviroment.data.getInfinitePlaneDistanceToOrigin();

        wrapper.physicsActor = PxCreatePlane(*getSDK(), PxPlane(normal.x, normal.y, normal.z, distToPlane), *sceneInfo.defaultMaterial);
        wrapper.physicsActor->userData = entity.data;

        sceneInfo.scene->addActor(*wrapper.physicsActor);
    }
}
