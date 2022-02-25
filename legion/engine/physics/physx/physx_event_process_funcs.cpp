#include "physx_event_process_funcs.hpp"
#include <physx/PxPhysicsAPI.h>
#include <physics/events/events.hpp>

using namespace physx;

namespace legion::physics
{
    void processAddBoxEvent(events::event_base* addBoxEvent, physx::PxScene* scene, PhysxInternalWrapper& wrapper, ecs::entity entity)
    {
        //cast event into actual derived event
        add_box_collider* boxColliderAddEvent = static_cast<add_box_collider*>(addBoxEvent);

        if (wrapper.bodyType == physics_body_type::rigidbody)
        {
            //PxRigidDynamic* box = PxCreateDynamic(*mPhysics, PxTransform(pos), PxBoxGeometry(dims), *mMaterial, density);
        }
        else if (wrapper.bodyType == physics_body_type::static_collider)
        {

        }
    }
}
