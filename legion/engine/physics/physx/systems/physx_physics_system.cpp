#include "physx_physics_system.hpp"
#include <physx/PxPhysicsAPI.h>
#include <physics/components/physics_component.hpp>
#include <physics/components/rigidbody.hpp>
#include <physics/events/events.hpp>
#include <physics/physx/physx_event_process_funcs.hpp>


namespace legion::physics
{
    using namespace physx;

    struct PhysxStatics
    {
        inline static size_type selfInstanceCounter = 0;

        inline static PxFoundation* foundation = nullptr;
        inline static PxPvd* pvd = nullptr;
        inline static PxDefaultCpuDispatcher* dispatcher = nullptr;
        inline static PxPhysics* phyxSDK = nullptr;

        inline static PxDefaultAllocator defaultAllocator;
        inline static PxDefaultErrorCallback defaultErrorCallback;
        inline static async::spinlock setupShutdownLock;
    };

    using PS = PhysxStatics;

    //PhysX PVD Debugger Related
    constexpr const char* pvdHost = "127.0.0.1";
    constexpr size_type defaultPVDListeningPort = 5425;
    constexpr size_type defaultPVDHostTimeout = 10;

    physx::PxPhysics* PhysXPhysicsSystem::getSDK()
    {
        return PS::phyxSDK;
    }

    void PhysXPhysicsSystem::setup()
    {
        {
            const async::lock_guard guard(PS::setupShutdownLock);

            PS::selfInstanceCounter++;

            lazyInitPhysXVariables();
            setupDefaultScene();
        }

        bindEventsToEventProcessors();
        createProcess<&PhysXPhysicsSystem::fixedUpdate>("Physics", m_timeStep);

        bindToEvent<events::component_destruction<physics_component>, &PhysXPhysicsSystem::markPhysicsWrapperPendingRemove>();
    }

    void PhysXPhysicsSystem::shutdown()
    {
        const async::lock_guard guard(PS::setupShutdownLock);

        m_physxWrapperContainer.ReleasePhysicsWrappers();

        m_physxScene->release();
        m_defaultMaterial->release();
        
        PS::selfInstanceCounter--;

        if (PS::selfInstanceCounter == 0)
        {
            releasePhysXVariables();
        }
    }

    void PhysXPhysicsSystem::lazyInitPhysXVariables()
    {
        if (PS::selfInstanceCounter == 1)
        {
            PS::foundation = PxCreateFoundation(PX_PHYSICS_VERSION,
                PS::defaultAllocator, PS::defaultErrorCallback);

            PS::pvd = PxCreatePvd(*PS::foundation);
            PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(pvdHost, defaultPVDListeningPort, defaultPVDHostTimeout);
            PS::pvd->connect(*transport, PxPvdInstrumentationFlag::eDEBUG);

            PS::dispatcher = PxDefaultCpuDispatcherCreate(0); //deal with multithreading later on

            PS::phyxSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *PS::foundation, PxTolerancesScale(), true, PS::pvd);
        }
    }

    void PhysXPhysicsSystem::setupDefaultScene()
    {
        PxSceneDesc sceneDesc(PS::phyxSDK->getTolerancesScale());
        sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
        sceneDesc.cpuDispatcher = PS::dispatcher;
        sceneDesc.filterShader = PxDefaultSimulationFilterShader;
        sceneDesc.flags |= PxSceneFlag::eENABLE_STABILIZATION;
        sceneDesc.flags |= PxSceneFlag::eENABLE_ACTIVE_ACTORS;

        m_physxScene = PS::phyxSDK->createScene(sceneDesc);
        m_defaultMaterial = PS::phyxSDK->createMaterial(0.5f, 0.5f, 0.1f);
    }

    void PhysXPhysicsSystem::bindEventsToEventProcessors()
    {
        m_physicsComponentActionFuncs[physics_component_flag::pc_add_first_box] = &processAddFirstBox;
        m_physicsComponentActionFuncs[physics_component_flag::pc_add_next_box] = &processAddNextBox;
        m_physicsComponentActionFuncs[physics_component_flag::pc_add_first_sphere] = &processAddFirstSphere;
        m_physicsComponentActionFuncs[physics_component_flag::pc_add_next_sphere] = &processAddNextSphere;

        m_rigidbodyComponentActionFuncs[rigidbody_flag::rb_velocity] = &processVelocityModification;
        m_rigidbodyComponentActionFuncs[rigidbody_flag::rb_mass] = &processMassModification;
    }

    void PhysXPhysicsSystem::releasePhysXVariables()
    {
        PS::dispatcher->release();
        PS::phyxSDK->release();

        if (PS::pvd)
        {
            PxPvdTransport* transport = PS::pvd->getTransport();
            PS::pvd->release();
            PS::pvd = nullptr;
            transport->release();
        }

        PS::foundation->release();
    }

    void PhysXPhysicsSystem::fixedUpdate(time::time_span<fast_time> deltaTime)
    {
        executePreSimulationActions();

        m_physxScene->simulate(m_timeStep);
        m_physxScene->fetchResults(true);

        executePostSimulationActions();
    }

    void PhysXPhysicsSystem::executePreSimulationActions()
    {
        //[1] Identify invalid entities and remove them from pxScene
        for (size_type idToRemove : m_wrapperPendingRemovalID)
        {
            m_physxWrapperContainer.PopAndSwapRemoveWrapper(idToRemove);
        }

        m_wrapperPendingRemovalID.clear();

        //[2] Identify new physics components
        ecs::filter<physics_component> physicsComponentFilter;

        for (auto entity : physicsComponentFilter)
        {
            auto& physComp = *entity.get_component<physics_component>();

            if (physComp.physicsComponentID == invalid_physics_component)
            {
                m_physxWrapperContainer.createPhysxWrapper(physComp);
            }
        }

        //[3] Identify new rigidbodies
        ecs::filter<physics_component,rigidbody> physicsAndRigidbodyComponentFilter;

        for (auto entity : physicsAndRigidbodyComponentFilter)
        {
            auto& physComp = *entity.get_component<physics_component>();
            pointer<PhysxInternalWrapper> physxWrapperPtr = m_physxWrapperContainer.findWrapperWithID(physComp.physicsComponentID);

            if (!physxWrapperPtr) { continue; }

            if (physxWrapperPtr->bodyType != physics::physics_body_type::rigidbody)
            {
                //the user has switched this body from static to dynamic
                physxWrapperPtr->bodyType = physics::physics_body_type::rigidbody;

                //TODO Reallocate this object to a dynamic object
            }
        }

        //[4] Go through events generated by physics_component and rigidbody
        PhysxEnviromentInfo enviromentInfo;
        enviromentInfo.scene = m_physxScene;
        enviromentInfo.defaultMaterial = m_defaultMaterial;

        for (auto entity : physicsComponentFilter)
        {
            auto& physComp = *entity.get_component<physics_component>();
            processPhysicsComponentEvents(entity, physComp, enviromentInfo);
        }

        for (auto entity : physicsAndRigidbodyComponentFilter)
        {
            auto& rbComp = *entity.get_component<rigidbody>();
            auto& physComp = *entity.get_component<physics_component>();

            processRigidbodyComponentEvents(entity, rbComp, physComp, enviromentInfo);
        }

        //[4] Physics Objects transformation may get directly modified by other systems
        // Given a list of entities that have been moved, transfer their movements to the respective physics actor
        //TODO identify entities whos transforms have been modified
    }

    void PhysXPhysicsSystem::executePostSimulationActions()
    {
        //[1] Transfer position changes from PxActor to actual entity position and rotation components
        PxU32 nbActiveActors;
        PxActor** activeActors = m_physxScene->getActiveActors(nbActiveActors);

        for (PxU32 i = 0; i < nbActiveActors; ++i)
        {
            PxActor* actor = activeActors[i];
            ecs::entity entity = { static_cast<ecs::entity_data*>(actor->userData) };

            if (!entity.valid()) { continue; }

            PxRigidActor* actorAsRigid = static_cast<PxRigidActor*>(actor);

            const PxTransform& pxTransform = actorAsRigid->getGlobalPose();

            const PxVec3& pxPosition = pxTransform.p;
            const PxQuat& pxRotation = pxTransform.q;

            *entity.get_component<position>() = math::vec3(pxPosition.x, pxPosition.y, pxPosition.z);
            *entity.get_component<rotation>() = math::quat(pxRotation.w, pxRotation.x, pxRotation.y, pxRotation.z);
        }
    }
    
    void PhysXPhysicsSystem::processPhysicsComponentEvents(ecs::entity ent, physics_component& physicsComponentToProcess, const PhysxEnviromentInfo& physicsEnviromentInfo)
    {
        const std::bitset<physics_component_flag::pc_max>& eventsGenerated = physicsComponentToProcess.physicsCompData.getGeneratedModifyEvents();

        core::pointer<PhysxInternalWrapper> wrapperPtr = m_physxWrapperContainer.findWrapperWithID(physicsComponentToProcess.physicsComponentID);

        if (!wrapperPtr) { return; }

        for(size_type bitPos = 0; bitPos < eventsGenerated.size(); ++bitPos)
        {
            if (eventsGenerated.test(bitPos))
            {
                m_physicsComponentActionFuncs[bitPos].invoke(physicsComponentToProcess, physicsEnviromentInfo, *wrapperPtr, ent);
            }
        }

        physicsComponentToProcess.physicsCompData.resetModificationFlags();
    }

    void PhysXPhysicsSystem::processRigidbodyComponentEvents(ecs::entity ent, rigidbody& rigidbody, physics_component& physicsComponentToProcess,
        const PhysxEnviromentInfo& physicsEnviromentInfo)
    {
        size_type physicsComponentID = physicsComponentToProcess.physicsComponentID;
        auto& eventsGenerated = rigidbody.rigidbodyData.getGeneratedModifyEvents();

        pointer<PhysxInternalWrapper> wrapperPtr = m_physxWrapperContainer.findWrapperWithID(physicsComponentID);

        if (!wrapperPtr) { return; }

        for (size_type bitPos = 0; bitPos < eventsGenerated.size(); ++bitPos)
        {
            if (eventsGenerated.test(bitPos))
            {
                m_rigidbodyComponentActionFuncs[bitPos].invoke(rigidbody, physicsEnviromentInfo, *wrapperPtr, ent);
            }
        }

        rigidbody.rigidbodyData.resetModificationFlags();
    }

    void PhysXPhysicsSystem::releasePhysXVariables()
    {
        PS::dispatcher->release();
        PS::phyxSDK->release();

        if (PS::pvd)
        {
            PxPvdTransport* transport = PS::pvd->getTransport();
            PS::pvd->release();
            PS::pvd = nullptr;
            transport->release();
        }

        PS::foundation->release();
    }

    void PhysXPhysicsSystem::fixedUpdate(time::time_span<fast_time> deltaTime)
    {
        m_physxScene->simulate(m_timeStep);
        m_physxScene->fetchResults(true);
    }
    
    void PhysXPhysicsSystem::processPhysicsComponentEvents(physicsComponent& physicsComponentToProcess)
    {

    }
}
