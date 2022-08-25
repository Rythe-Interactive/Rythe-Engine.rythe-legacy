#pragma once
#include <physx/PxPhysicsAPI.h>
#include <physics/components/capsule_controller.hpp>

namespace legion::physics
{
    using namespace physx;

    class ControllerHitFeedback : public PxUserControllerHitReport
    {
    public:
        void setShapeHitDelegate(delegate<void(const PxControllerShapeHit&)> shapeHitDelegate)
        {
            m_onShapeHit = shapeHitDelegate;
        }

        void setControllerHitDelegate(delegate<void(const PxControllerHit& hit)> controllerHitDelegate)
        {
            m_onControllerHit = controllerHitDelegate;
        }

        void setObstacleHitDelegate(delegate<void(const PxControllerObstacleHit& hit)> obstacleHitDelegate)
        {
            m_onObstacleHit = obstacleHitDelegate;
        }

        virtual void onShapeHit(const PxControllerShapeHit& hit) override
        {
            m_onShapeHit.invoke(hit);
        }

        //brief Called when current controller hits another controller.
        virtual void onControllerHit(const PxControllersHit& hit) override
        {
            m_onControllerHit.invoke(hit);
        }

        //Called when current controller hits a user-defined obstacle.
        virtual void onObstacleHit(const PxControllerObstacleHit& hit) override
        {
            m_onObstacleHit.invoke(hit);
        }

    private:

        delegate<void(const PxControllerShapeHit&)> m_onShapeHit;
        delegate<void(const PxControllerHit& hit)> m_onControllerHit;
        delegate<void(const PxControllerObstacleHit& hit)> m_onObstacleHit;
    };
}
