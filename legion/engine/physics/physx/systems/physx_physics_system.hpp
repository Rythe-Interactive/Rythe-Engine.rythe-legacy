#pragma once
#include <core/core.hpp>
#include <physics/physx/data/physx_wrapper_container.hpp>

namespace physx
{
    class PxScene;
    class PxPhysics;
    class PxMaterial;
};

namespace legion::physics
{
    struct PhysxEnviromentInfo
    {
        physx::PxScene* scene = nullptr;
        physx::PxMaterial* defaultMaterial = nullptr;
    };

    class PhysXPhysicsSystem final : public System<PhysXPhysicsSystem>
    {
    public:

        virtual void setup();

        virtual void shutdown();

        void fixedUpdate(time::time_span<fast_time> deltaTime);

        static physx::PxPhysics* getSDK();

    private:

        struct PhysxStatics;

        void lazyInitPhysXVariables();

        void releasePhysXVariables();

        void setupDefaultScene();

        void executePreSimulationActions();

        void executePostSimulationActions();

        void processPhysicsComponentEvents( physicsComponent& physicsComponentToProcess );

        static constexpr float m_timeStep = 0.02f;

        physx::PxScene* m_physxScene;
        physx::PxMaterial* m_defaultMaterial = nullptr;

        PhysxWrapperContainer m_physxWrapperContainer;

        std::mutex m_setupShutdownMutex;

        using physicsEventProcessFunc = delegate<void(core::events::event_base*, PhysxEnviromentInfo&, PhysxInternalWrapper&, ecs::entity)>;

        std::unordered_map<size_t, physicsEventProcessFunc> m_eventHashToPCEventProcess;
    };
};
