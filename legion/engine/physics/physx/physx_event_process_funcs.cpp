#include "physx_event_process_funcs.hpp"
#include <physics/physx/physx_integration_helpers.hpp>
#include <physx/PxPhysicsAPI.h>
#include <physics/events/events.hpp>

using namespace physx;

namespace legion::physics
{
    void processVelocityChange(const core::events::event_base& velocityChangeEvent, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity)
    {
        const rb_modify_velocity& modifyVelocity = static_cast<const rb_modify_velocity&>(velocityChangeEvent);
        const math::vec3& vel = modifyVelocity.newVelocity;

        if (wrapper.bodyType == physics_body_type::rigidbody)
        {
            for (PxActor* actor : wrapper.physicsActors)
            {
                PxRigidDynamic* rigid = static_cast<PxRigidDynamic*>(actor);

                PxVec3 pxVelocity(vel.x, vel.y, vel.z);
                rigid->setLinearVelocity(pxVelocity);
            }
        }
    }

    void processAddBoxEvent(events::event_base* addBoxEvent, const PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity)
    {
        //cast event into actual derived event
        add_box_collider* boxColliderAddEvent = static_cast<add_box_collider*>(addBoxEvent);

        const math::vec3& pos = *entity.get_component<position>();
        const math::quat& rot = *entity.get_component<rotation>();

        PxTransform transform(
            PxVec3(pos.x, pos.y, pos.z),
            PxQuat(rot.x, rot.y, rot.z,rot.w));

        const math::vec3& extents = boxColliderAddEvent->newExtents;

        if (wrapper.bodyType == physics_body_type::rigidbody)
        {
            PxRigidDynamic* rb = PxCreateDynamic(*getSDK(), transform,
                PxBoxGeometry(extents.x, extents.y, extents.z), *sceneInfo.defaultMaterial, 1.0f);

            rb->userData = entity.data;

            wrapper.physicsActors.push_back(rb);
            sceneInfo.scene->addActor(*rb);
        }
        else if (wrapper.bodyType == physics_body_type::static_collider)
        {
            PxRigidStatic* staticCollider = PxCreateStatic(*getSDK(), transform,
                PxBoxGeometry(extents.x, extents.y, extents.z), *sceneInfo.defaultMaterial);

            wrapper.physicsActors.push_back(staticCollider);
            sceneInfo.scene->addActor(*staticCollider);
        }


    }
}
