#include <physics/physx/physx_event_process_funcs.hpp>
#include <physx/PxPhysicsAPI.h>
#include <physics/physx/physx_integration_helpers.hpp>
#include <physics/events/events.hpp>
#include <physics/components/physics_component.hpp>
#include <physics/components/capsule_controller.hpp>
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

            BoxColliderData& box = collider.getColliderAsBox();

            PxTransform localTransform;
            calculateLocalColliderTransform(localTransform, collider);

            const math::vec3& extents = box.getBoxExtents();

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
            
            BoxColliderData& box = collider.getColliderAsBox();

            PxTransform transform;
            PxTransform localTransform;

            calculateGlobalAndLocalTransforms(localTransform, transform, collider, entity);

            const math::vec3& extents = box.getBoxExtents();

            if (wrapper.bodyType == physics_body_type::rigidbody)
            {
                instantiateDynamicActorWith<PxBoxGeometry, const PxVec3&>(getSDK(), wrapper, transform,
                    localTransform, sceneInfo, entity, PxVec3(extents.x, extents.y, extents.z));
            }
            else if (wrapper.bodyType == physics_body_type::static_collider)
            {
                instantiateStaticActorWith<PxBoxGeometry, const PxVec3&>(getSDK(), wrapper, transform,
                    localTransform, sceneInfo, entity, PxVec3(extents.x, extents.y, extents.z));
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

            float radius = collider.getColliderAsSphere().getRadius();

            if (wrapper.bodyType == physics_body_type::rigidbody)
            {
                instantiateDynamicActorWith<PxSphereGeometry,float&>(getSDK(), wrapper, transform, localTransform, sceneInfo, entity, radius);
            }
            else if (wrapper.bodyType == physics_body_type::static_collider)
            {
                instantiateStaticActorWith<PxSphereGeometry, float&>(getSDK(), wrapper, transform, localTransform, sceneInfo, entity, radius);
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

            float radius = collider.getColliderAsSphere().getRadius();

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

            ConvexColliderData& convex = collider.getColliderAsConvex();
            PxConvexMesh* convexMesh = static_cast<PxConvexMesh*>(convex.getConvexPtr());

            if (wrapper.bodyType == physics_body_type::rigidbody)
            {
                instantiateDynamicActorWith<PxConvexMeshGeometry, PxConvexMesh*&>(getSDK(), wrapper, transform, localTransform, sceneInfo, entity, convexMesh);
            }
            else if (wrapper.bodyType == physics_body_type::static_collider)
            {
                instantiateStaticActorWith<PxConvexMeshGeometry, PxConvexMesh*&>(getSDK(), wrapper, transform, localTransform, sceneInfo, entity, convexMesh);
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

            ConvexColliderData& convex = collider.getColliderAsConvex();
            PxConvexMesh* convexMesh = static_cast<PxConvexMesh*>(convex.getConvexPtr());

            instantiateNextCollider<PxConvexMeshGeometry, PxConvexMesh*&>(getSDK(), wrapper, localTransform, sceneInfo, convexMesh);
        }
    }

    void processSetPhysicsMaterial(const ColliderData& collider, const collider_modification_data& modData, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper)
    {
        PxRigidActor* rigid = static_cast<PxRigidActor*>(wrapper.physicsActor);

        PxShape* shapes; PxU32 shapeCount = rigid->getNbShapes();
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

    void processAddInfinitePlane(physics_enviroment& physicsEnviroment, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity)
    {
        const math::vec3& normal = physicsEnviroment.data.getInfinitePlaneNormal();
        float distToPlane = physicsEnviroment.data.getInfinitePlaneDistanceToOrigin();

        wrapper.physicsActor = PxCreatePlane(*getSDK(), PxPlane(normal.x, normal.y, normal.z, distToPlane), *sceneInfo.defaultMaterial);
        wrapper.physicsActor->userData = entity.data;

        sceneInfo.scene->addActor(*wrapper.physicsActor);
    }

    void processCapsuleMoveTo(PhysxCharacterWrapper& characterWrapper, capsule_controller& capsule)
    {
        CapsuleControllerData& capsuleData = capsule.data;

        const math::vec3& disp = capsuleData.getCurrentDisplacement();
        log::debug("displaced by {0}", math::to_string(disp));
        characterWrapper.characterController->move(PxVec3{ disp.x,disp.y,disp.z }, 0.0f, 0.016f, PxControllerFilters());
        capsuleData.resetDisplacement();
    }
}
