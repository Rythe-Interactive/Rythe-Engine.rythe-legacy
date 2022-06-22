#pragma once
#include <core/core.hpp>
#include <application/application.hpp>
#include <physics/events/events.hpp>

namespace lgn = legion;

struct RequestFlipPhysicsContinuousState : public lgn::app::input_action<RequestFlipPhysicsContinuousState> {};
struct RequestSinglePhysicsTick : public lgn::app::input_action<RequestSinglePhysicsTick> {};
struct RequestRaycastDebug : public lgn::app::input_action<RequestRaycastDebug> {};

namespace legion::physics
{
    class PhysicsDebuggingSystem final : public System<PhysicsDebuggingSystem>
    {
    public:

        virtual void setup();

    private:

        void raiseFlipContinuousUpdateEvent(RequestFlipPhysicsContinuousState& action);

        void raiseSinglePhysicsTickEvent(RequestSinglePhysicsTick& action);

        bool m_isPhysicsContinuousUpdate = true;
    };
};
