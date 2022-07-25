#include "physics_debugging_system.hpp"
#include <rendering/rendering.hpp>

namespace legion::physics
{
    void PhysicsDebuggingSystem::setup()
    {
        app::InputSystem::createBinding<RequestFlipPhysicsContinuousState>(app::inputmap::method::M);
        app::InputSystem::createBinding<RequestSinglePhysicsTick>(app::inputmap::method::N);

        bindToEvent<RequestFlipPhysicsContinuousState,
            &PhysicsDebuggingSystem::raiseFlipContinuousUpdateEvent>();

        bindToEvent<RequestSinglePhysicsTick,
            &PhysicsDebuggingSystem::raiseSinglePhysicsTickEvent>();
    }

    void PhysicsDebuggingSystem::raiseFlipContinuousUpdateEvent(RequestFlipPhysicsContinuousState& action)
    {
        if (action.value)
        {
            m_isPhysicsContinuousUpdate = !m_isPhysicsContinuousUpdate;
            raiseEvent<request_flip_physics_continuous>(request_flip_physics_continuous{ m_isPhysicsContinuousUpdate });
        }
    }

    void PhysicsDebuggingSystem::raiseSinglePhysicsTickEvent(RequestSinglePhysicsTick& action)
    {
        if (!m_isPhysicsContinuousUpdate && action.value)
        {
            raiseEvent<request_single_physics_tick>();
        }
    }
}
