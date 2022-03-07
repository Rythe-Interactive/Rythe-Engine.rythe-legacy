#pragma once
#include <core/core.hpp>
#include <physics/physx/data/physx_wrapper_container.hpp>
#include <physics/physx/physx_integration_helpers.hpp>
#include <core/containers/delegate.hpp>
#include <physics/components/rigidbody.hpp>

namespace physx
{
    class PxScene;
    class PxPhysics;
    class PxMaterial;
};

namespace legion::physics
{
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

        void bindEventsToEventProcessors();

        inline void markPhysicsWrapperPendingRemove(events::component_destruction<physicsComponent>& event)
        {
            m_wrapperPendingRemovalID.push_back(event.entity.get_component<physicsComponent>()->physicsComponentID);
        }

        void executePreSimulationActions();

        void executePostSimulationActions();

        void processPhysicsComponentEvents(ecs::entity ent, physicsComponent& physicsComponentToProcess, const PhysxEnviromentInfo& physicsEnviromentInfo);

        void processRigidbodyComponentEvents(ecs::entity ent, size_type wrapperID, rigidbody& rigidbody, const PhysxEnviromentInfo& physicsEnviromentInfo);

        static constexpr float m_timeStep = 0.02f;

        physx::PxScene* m_physxScene;
        physx::PxMaterial* m_defaultMaterial = nullptr;

        PhysxWrapperContainer m_physxWrapperContainer;

        std::mutex m_setupShutdownMutex;

        using pcEventProcessFunc = delegate<void(physicsComponent&,const PhysxEnviromentInfo&, PhysxInternalWrapper&, ecs::entity)>;
        std::array< pcEventProcessFunc, physics_component_flag::pc_max> m_physicsComponentActionFuncs;

        using rbEventProcessFunc = delegate<void(const core::events::event_base&, const PhysxEnviromentInfo&, PhysxInternalWrapper&, ecs::entity)>;
        std::unordered_map<id_type, rbEventProcessFunc > m_eventHashToRBEventProcess;

        std::vector<size_type> m_wrapperPendingRemovalID;
    };
};
