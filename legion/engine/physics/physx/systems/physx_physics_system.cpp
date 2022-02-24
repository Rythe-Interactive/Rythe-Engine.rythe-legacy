#include "physx_physics_system.hpp"
#include <physx/PxPhysicsAPI.h>
#include <physics/components/physics_component.hpp>
#include <physics/components/rigidbody.hpp>

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

    void PhysXPhysicsSystem::setup()
    {
        {
            const async::lock_guard guard(PS::setupShutdownLock);

            PS::selfInstanceCounter++;

            lazyInitPhysXVariables();
            setupDefaultScene();
        }
        
        createProcess<&PhysXPhysicsSystem::fixedUpdate>("Physics", m_timeStep);
    }

    void PhysXPhysicsSystem::shutdown()
    {
        const async::lock_guard guard(PS::setupShutdownLock);

        m_physxScene->release();
        
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

        m_physxScene = PS::phyxSDK->createScene(sceneDesc);
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
        //iterate through all entities that have a physics component
            //if an unregistered physics component is found
                //create physx wrapper
        ecs::filter<physicsComponent> physicsComponentFilter;

        for (auto entity : physicsComponentFilter)
        {
            auto& physComp = *entity.get_component<physicsComponent>();

            if (physComp.physicsComponentID == invalid_physics_component)
            {
                m_physxWrapperContainer.createPhysxWrapper(physComp);
            }
        }

        ecs::filter<physicsComponent,rigidbody> physicsAndRigidbodyComponentFilter;

        for (auto entity : physicsAndRigidbodyComponentFilter)
        {
            auto& rbComp = *entity.get_component<rigidbody>();

            if (rbComp.rigidbodyIndex)
            {
                //m_physxWrapperContainer.CreatePhysxWrapper(physComp);
            }
        }

        //at this point all physics components and rigidbodies are registered, we can go through its events now
            //
    }

    void PhysXPhysicsSystem::executePostSimulationActions()
    {
        //update positions of actors

        //go through actor void pointers
            //if entity 
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
}
