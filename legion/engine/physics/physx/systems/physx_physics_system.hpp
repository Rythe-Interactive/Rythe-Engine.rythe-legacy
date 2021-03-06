#pragma once
#include <core/core.hpp>
#include <physics/physx/data/physx_wrapper_container.hpp>
#include <physics/physx/physx_integration_helpers.hpp>
#include <core/containers/delegate.hpp>
#include <physics/components/rigidbody.hpp>
#include <physics/physics_helpers.hpp>>

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

        void physicsStep();

        void update(legion::time::span deltaTime);

        static physx::PxPhysics* getSDK();

    private:

        float m_accumulation;
        
        static void* physxGenerateConvexMesh(const std::vector<math::vec3>& vertices);

        void lazyInitPhysXVariables();

        void releasePhysXVariables();

        void setupDefaultScene();

        void bindEventsToEventProcessors();

        inline void markPhysicsWrapperPendingRemove(events::component_destruction<physics_component>& event)
        {
            m_wrapperPendingRemovalID.push_back(event.entity.get_component<physics_component>()->physicsComponentID);
        }

        void onRequestCreatePhysicsMaterial(request_create_physics_material& physicsMaterialRequest);

        void executePreSimulationActions();

        void executePostSimulationActions();

        void processPhysicsComponentEvents(ecs::entity ent, physics_component& physicsComponentToProcess, const PhysxEnviromentInfo& physicsEnviromentInfo);

        void processRigidbodyComponentEvents(ecs::entity ent, rigidbody& rigidbody, physics_component& physicsComponentToProcess, const PhysxEnviromentInfo& physicsEnviromentInfo);

        void processColliderModificationEvents(physics_component& physicsComponentToProcess, const PhysxEnviromentInfo& physicsEnviromentInfo);

        void processPhysicsEnviromentEvents(ecs::entity ent, physics_enviroment& physicsComponentToProcess, const PhysxEnviromentInfo& physicsEnviromentInfo);

        static constexpr float m_timeStep = 0.02f;
        static constexpr size_type m_maxPhysicsStep = 3;

        physx::PxScene* m_physxScene;

        PhysxWrapperContainer m_physxWrapperContainer;

        using pcEventProcessFunc = delegate<void(physics_component&,const PhysxEnviromentInfo&, PhysxInternalWrapper&, ecs::entity)>;
        std::array< pcEventProcessFunc, physics_component_flag::pc_max> m_physicsComponentActionFuncs;

        using rbEventProcessFunc = delegate<void(rigidbody&,const PhysxEnviromentInfo&, PhysxInternalWrapper&, ecs::entity)>;
        std::array< rbEventProcessFunc, rigidbody_flag::rb_max> m_rigidbodyComponentActionFuncs;

        using peEventProcessFunc = delegate<void(physics_enviroment&, const PhysxEnviromentInfo&, PhysxInternalWrapper&, ecs::entity)>;
        std::array< peEventProcessFunc, physics_enviroment_flag::pe_max> m_enviromentComponentActionFuncs;

        using cmEventProcessFunc = delegate<void(const ColliderData&, const collider_modification_data&, const PhysxEnviromentInfo&, PhysxInternalWrapper&)>;
        std::array<cmEventProcessFunc, collider_modification_flag::cm_max> m_colliderActionFuncs;

        std::vector<size_type> m_wrapperPendingRemovalID;
        std::unordered_map<physics_material_hash, physx::PxMaterial*> m_physicsMaterials;

        //------------------------------------------------ Debugging Related --------------------------------------------------------------//

        bool m_isContinuousStepActive = true;
        bool m_isSingleStepContinueAcitve = false;

        void flipPhysicsContinuousState(request_flip_physics_continuous& request) { m_isContinuousStepActive = request.newContinuousState; }

        void activateSingleStepContinue(request_single_physics_tick& request) { m_isSingleStepContinueAcitve = true; }
    };
};
