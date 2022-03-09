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

        for (ConvexColliderData& convexCollider : data.GetConvexData())
        {
            bool isUnRegisteredBox = !convexCollider.isDataRead() && convexCollider.getConvexType() == convex_type::box;
            convexCollider.setDataRead(true);

            const math::vec3& pos = *entity.get_component<position>();
            const math::quat& rot = *entity.get_component<rotation>();

            const math::vec3& localOffset = convexCollider.getOffset();
            const math::quat& localRot = convexCollider.getRotationOffset();

            PxTransform transform(
                PxVec3(pos.x, pos.y, pos.z),
                PxQuat(rot.x, rot.y, rot.z, rot.w));

            PxTransform localTransform(
                PxVec3(localOffset.x, localOffset.y, localOffset.z),
                PxQuat(localRot.x, localRot.y, localRot.z, localRot.w));

            const math::vec3& extents = convexCollider.getBoxExtents();

            if (isUnRegisteredBox)
            {
                PxShape* shape = getSDK()->createShape(
                    PxBoxGeometry(PxVec3(extents.x, extents.y, extents.z)), *sceneInfo.defaultMaterial, true);

                PxRigidActor* rigid = static_cast<PxRigidActor*>(wrapper.physicsActor);
                
                shape->setLocalPose(localTransform);
                rigid->attachShape(*shape);
                shape->release();
            }
        }

    }

    void processAddFirstBox(physicsComponent& physicsComponent, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity)
    {
        physicsComponentData& data = physicsComponent.physicsCompData;

        for (ConvexColliderData& convexCollider : data.GetConvexData())
        {
            bool isUnRegisteredBox = !convexCollider.isDataRead() && convexCollider.getConvexType() == convex_type::box;
            convexCollider.setDataRead(true);

            if (isUnRegisteredBox)
            {
                const math::vec3& pos = *entity.get_component<position>();
                const math::quat& rot = *entity.get_component<rotation>();

                const math::vec3& localOffset = convexCollider.getOffset();
                const math::quat& localRot = convexCollider.getRotationOffset();

                PxTransform transform(
                    PxVec3(pos.x, pos.y, pos.z),
                    PxQuat(rot.x, rot.y, rot.z, rot.w));

                PxTransform localTransform(
                    PxVec3(localOffset.x, localOffset.y, localOffset.z),
                    PxQuat(localRot.x, localRot.y, localRot.z, localRot.w));

                const math::vec3& extents = convexCollider.getBoxExtents();

                if (wrapper.bodyType == physics_body_type::rigidbody)
                {
                    PxRigidDynamic* rb = PxCreateDynamic(*getSDK(), transform,
                        PxBoxGeometry(extents.x, extents.y, extents.z), *sceneInfo.defaultMaterial, 1.0f, localTransform);
                    rb->userData = entity.data;

                    wrapper.physicsActor = rb;
                    sceneInfo.scene->addActor(*rb);
                }
                else if (wrapper.bodyType == physics_body_type::static_collider)
                {
                    PxRigidStatic* staticCollider = PxCreateStatic(*getSDK(), transform,
                        PxBoxGeometry(extents.x, extents.y, extents.z), *sceneInfo.defaultMaterial, localTransform);

                    wrapper.physicsActor = staticCollider;
                    sceneInfo.scene->addActor(*staticCollider);
                }
            }

            break;
        }
    }
}
