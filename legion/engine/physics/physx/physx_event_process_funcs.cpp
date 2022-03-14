#include <physics/physx/physx_event_process_funcs.hpp>
#include <physx/PxPhysicsAPI.h>
#include <physics/physx/physx_integration_helpers.hpp>
#include <physics/events/events.hpp>
#include <physics/components/physics_component.hpp>
#include <physics/physx/physx_event_process_funcs.hpp>

using namespace physx;

namespace legion::physics
{
    void processVelocityModification(const core::events::event_base& velocityChangeEvent, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity)
    {
        const rb_modify_velocity& modifyVelocity = static_cast<const rb_modify_velocity&>(velocityChangeEvent);
        const math::vec3& vel = modifyVelocity.newVelocity;

        if (wrapper.bodyType == physics_body_type::rigidbody)
        {
            PxRigidDynamic* rigid = static_cast<PxRigidDynamic*>(wrapper.physicsActor);

            PxVec3 pxVelocity(vel.x, vel.y, vel.z);
            rigid->setLinearVelocity(pxVelocity);
        }
    }
    
    void processAddNextBox(physicsComponent& physicsComponent, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity)
    {
        physicsComponentData& data = physicsComponent.physicsCompData;

        for (ConvexColliderData& convexCollider : data.getConvexData())
        {
            if (convexCollider.isRegisteredOfType(collider_type::box)) { continue; }

            convexCollider.setRegistered(true);

            PxTransform transform;
            PxTransform localTransform;

            calculateGlobalAndLocalTransforms(localTransform, transform, convexCollider, entity);

            const math::vec3& extents = convexCollider.getBoxExtents();

            instantiateNextCollider<PxBoxGeometry,const PxVec3&>(getSDK(), wrapper, transform, localTransform, sceneInfo, PxVec3(extents.x, extents.y, extents.z));
        }
    }
    
    void processAddFirstBox(physicsComponent& physicsComponent, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity)
    {
        physicsComponentData& data = physicsComponent.physicsCompData;

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

    void processAddFirstSphere(physicsComponent& physicsComponent, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity)
    {
        physicsComponentData& data = physicsComponent.physicsCompData;

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
                instantiateDynamicActorWith<PxSphereGeometry>(getSDK(), wrapper, transform, localTransform, sceneInfo, entity, radius);
            }
            else if (wrapper.bodyType == physics_body_type::static_collider)
            {
                instantiateStaticActorWith<PxSphereGeometry>(getSDK(), wrapper, transform, localTransform, sceneInfo, entity, radius);
            }

            break;
        }
    }

    void processAddNextSphere(physicsComponent& physicsComponent, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity)
    {
        physicsComponentData& data = physicsComponent.physicsCompData;

        for (SphereColliderData& sphereCollider : data.getSphereData())
        {
            if (sphereCollider.isRegisteredOfType(collider_type::sphere)) { continue; }

            sphereCollider.setRegistered(true);

            PxTransform transform;
            PxTransform localTransform;

            calculateGlobalAndLocalTransforms(localTransform, transform, sphereCollider, entity);

            float radius = sphereCollider.getRadius();

            instantiateNextCollider<PxSphereGeometry>(getSDK(), wrapper, transform, localTransform, sceneInfo, radius);
        }
    }
}
