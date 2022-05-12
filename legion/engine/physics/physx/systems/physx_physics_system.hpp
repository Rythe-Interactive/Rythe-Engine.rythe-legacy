#pragma once
#include <core/core.hpp>
#include <physics/physx/data/physx_wrapper_container.hpp>
#include <physics/physx/data/physx_wrapper.hpp>
#include <physics/physx/physx_integration_helpers.hpp>
#include <core/containers/delegate.hpp>
#include <physics/components/rigidbody.hpp>
#include <physics/components/physics_component.hpp>
#include <physics/physics_helpers.hpp>

namespace physx
{
    class PxVec3;
    class PxScene;
    class PxControllerManager;
    class PxPhysics;
    class PxMaterial;
    class PxController;
    struct PxControllerShapeHit;
};

namespace legion::physics
{
    class CapsuleControllerData;
    struct controller_preset;
   
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

        void markPhysicsWrapperPendingRemove(events::component_destruction<physics_component>& event);
        
        void onRequestCreatePhysicsMaterial(request_create_physics_material& physicsMaterialRequest);

        void executePreSimulationActions();

        void executePostSimulationActions();

        void executePreTimeStepActions();

        void instantiateCharacterController(ecs::entity ent,const CapsuleControllerData& capsuleData, PhysxCharacterWrapper& outCharacterWrapper);

        void characterControllerSweep(ecs::entity characterEnt, PhysxCharacterWrapper& character, const physx::PxVec3& displacement);

        void processPhysicsComponentEvents(ecs::entity ent, physics_component& physicsComponentToProcess, const PhysxEnviromentInfo& physicsEnviromentInfo);

        void processRigidbodyComponentEvents(ecs::entity ent, rigidbody& rigidbody, physics_component& physicsComponentToProcess, const PhysxEnviromentInfo& physicsEnviromentInfo);

        void processColliderModificationEvents(physics_component& physicsComponentToProcess, const PhysxEnviromentInfo& physicsEnviromentInfo);

        void processCapsuleCharacterModificationEvents(capsule_controller& capsule);

        void processPhysicsEnviromentEvents(ecs::entity ent, physics_enviroment& physicsComponentToProcess, const PhysxEnviromentInfo& physicsEnviromentInfo);

        delegate<void(const physx::PxControllerShapeHit&)> initializeDefaultRigidbodyToCharacterResponse(float forceAmount, float massMaximum);

        static constexpr float m_timeStep = 0.02f;
        static constexpr size_type m_maxPhysicsStep = 3;

        size_type m_currentSteptickAmount = 0;

        physx::PxScene* m_physxScene = nullptr;
        physx::PxControllerManager* m_characterManager = nullptr;

        PhysxWrapperContainer<PhysxInternalWrapper> m_physxWrapperContainer;
        PhysxWrapperContainer<PhysxCharacterWrapper> m_characterContainer;

        using pcEventProcessFunc = delegate<void(physics_component&,const PhysxEnviromentInfo&, PhysxInternalWrapper&, ecs::entity)>;
        std::array< pcEventProcessFunc, physics_component_flag::pc_max> m_physicsComponentActionFuncs;

        using rbEventProcessFunc = delegate<void(rigidbody&,const PhysxEnviromentInfo&, PhysxInternalWrapper&, ecs::entity)>;
        std::array< rbEventProcessFunc, rigidbody_flag::rb_max> m_rigidbodyComponentActionFuncs;

        using peEventProcessFunc = delegate<void(physics_enviroment&, const PhysxEnviromentInfo&, PhysxInternalWrapper&, ecs::entity)>;
        std::array< peEventProcessFunc, physics_enviroment_flag::pe_max> m_enviromentComponentActionFuncs;

        using cmEventProcessFunc = delegate<void(const ColliderData&, const collider_modification_data&, const PhysxEnviromentInfo&, PhysxInternalWrapper&)>;
        std::array<cmEventProcessFunc, collider_modification_flag::cm_max> m_colliderActionFuncs;

        using ccEventProcessFunc = delegate<void(PhysxCharacterWrapper&, capsule_controller&) > ;
        std::array<ccEventProcessFunc, capsule_character_flag::cc_max> m_capsuleActionFuncs;

        using characterPresetProcessFunc = delegate<void(controller_preset&, PhysxCharacterWrapper&, const PhysxEnviromentInfo&)>;
        std::unordered_map<size_type,characterPresetProcessFunc> m_hashToPresetProcessFunc;

        std::vector<size_type> m_wrapperPendingRemovalID;

        std::unordered_map<size_type, physx::PxMaterial*> m_physicsMaterials;

        //------------------------------------------------ Debugging Related --------------------------------------------------------------//

        bool m_isContinuousStepActive = true;
        bool m_isSingleStepContinueAcitve = false;

        void flipPhysicsContinuousState(request_flip_physics_continuous& request) { m_isContinuousStepActive = request.newContinuousState; }

        void activateSingleStepContinue(request_single_physics_tick& request) { m_isSingleStepContinueAcitve = true; }
    };
};
