#pragma once
#include <core/core.hpp>
#include <physics/physx/data/physx_wrapper_container.hpp>

namespace physx
{
    class PxScene;
};

namespace legion::physics
{
    class PhysXPhysicsSystem final : public System<PhysXPhysicsSystem>
    {
    public:

        virtual void setup();

        virtual void shutdown();

        void fixedUpdate(time::time_span<fast_time> deltaTime);

    private:

        struct PhysxStatics;

        void lazyInitPhysXVariables();

        void releasePhysXVariables();

        void setupDefaultScene();

        void executePreSimulationActions();

        void executePostSimulationActions();

        static constexpr float m_timeStep = 0.02f;

        physx::PxScene* m_physxScene;
        PhysxWrapperContainer m_physxWrapperContainer;

        std::mutex m_setupShutdownMutex;
    };
};
