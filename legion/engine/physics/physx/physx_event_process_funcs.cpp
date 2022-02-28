#include "physx_event_process_funcs.hpp"
#include <physics/physx/systems/physx_physics_system.hpp>
#include <physx/PxPhysicsAPI.h>
#include <physics/events/events.hpp>

using namespace physx;

namespace legion::physics
{
    void processAddBoxEvent(events::event_base* addBoxEvent, PhysxEnviromentInfo& sceneInfo, PhysxInternalWrapper& wrapper, ecs::entity entity)
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
            PxRigidDynamic* box = PxCreateDynamic(*PhysXPhysicsSystem::getSDK(), transform,
                PxBoxGeometry(extents.x, extents.y, extents.z), *sceneInfo.defaultMaterial, 1.0f);
        }
        else if (wrapper.bodyType == physics_body_type::static_collider)
        {

        }
    }
}
