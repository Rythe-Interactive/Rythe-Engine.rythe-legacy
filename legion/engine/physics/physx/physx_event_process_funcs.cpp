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
    void processVelocityModification(rigidbody& rigidbody, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity)
    {
        const math::vec3& vel = rigidbody.data.getVelocity();

        PxRigidDynamic* rigid = static_cast<PxRigidDynamic*>(wrapper.physicsActor);
        
        PxVec3 pxVelocity(vel.x, vel.y, vel.z);
        rigid->setLinearVelocity(pxVelocity);
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
        rigidbody.data.setDensity(newDensity);
    }

    void processAddNextBox(physics_component& physicsComponent, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity)
    {
        PhysicsComponentData& data = physicsComponent.physicsCompData;

        for (ConvexColliderData& convexCollider : data.getConvexData())
        {
            if (convexCollider.isRegisteredOfType(collider_type::box)) { continue; }

            convexCollider.setRegistered(true);

            PxTransform localTransform;
            calculateLocalColliderTransform(localTransform, convexCollider);

            const math::vec3& extents = convexCollider.getBoxExtents();

            instantiateNextCollider<PxBoxGeometry,const PxVec3&>(getSDK(), wrapper, localTransform, sceneInfo, PxVec3(extents.x, extents.y, extents.z));
        }
    }
    
    void processAddFirstBox(physics_component& physicsComponent, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity)
    {
        PhysicsComponentData& data = physicsComponent.physicsCompData;

        for (ConvexColliderData& convexCollider : data.getConvexData())
        {
            if (convexCollider.isRegisteredOfType(collider_type::box)) { continue; }

            convexCollider.setRegistered(true);

            PxTransform transform;
            PxTransform localTransform;

            calculateGlobalAndLocalTransforms(localTransform, transform, convexCollider, entity);

            const math::vec3& extents = convexCollider.getBoxExtents();

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

        for (SphereColliderData& sphereCollider : data.getSphereData())
        {
            if (sphereCollider.isRegisteredOfType(collider_type::sphere)) { continue; }

            sphereCollider.setRegistered(true);

            PxTransform transform;
            PxTransform localTransform;

            calculateGlobalAndLocalTransforms(localTransform, transform, sphereCollider, entity);

            float radius = sphereCollider.getRadius();

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

        for (SphereColliderData& sphereCollider : data.getSphereData())
        {
            if (sphereCollider.isRegisteredOfType(collider_type::sphere)) { continue; }

            sphereCollider.setRegistered(true);

            PxTransform localTransform;
            calculateLocalColliderTransform(localTransform, sphereCollider);

            float radius = sphereCollider.getRadius();

            instantiateNextCollider<PxSphereGeometry, float&>(getSDK(), wrapper, localTransform, sceneInfo, radius);
        }
    }

    void processAddFirstConvex(physics_component& physicsComponent, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity)
    {
        PhysicsComponentData& data = physicsComponent.physicsCompData;

        for (ConvexColliderData& convex : data.getConvexData())
        {
            if (convex.isRegisteredOfType(collider_type::quickhull_convex)) { continue; }

            convex.setRegistered(true);

            PxTransform transform;
            PxTransform localTransform;

            calculateGlobalAndLocalTransforms(localTransform, transform, convex, entity);
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

        for (ConvexColliderData& convex : data.getConvexData())
        {
            if (!convex.isRegisteredOfType(collider_type::quickhull_convex)) { continue; }

            convex.setRegistered(true);

            PxTransform localTransform;
            calculateLocalColliderTransform(localTransform, convex);

            PxConvexMesh* convexMesh = static_cast<PxConvexMesh*>(convex.getConvexPtr());
            instantiateNextCollider<PxConvexMeshGeometry, PxConvexMesh*&>(getSDK(), wrapper, localTransform, sceneInfo, convexMesh);
        }
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
