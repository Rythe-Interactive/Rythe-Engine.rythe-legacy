#include "physx_physics_system.hpp"
#include <physx/PxPhysicsAPI.h>
#include <filesystem>
#include <physics/components/physics_component.hpp>
#include <physics/components/rigidbody.hpp>
#include <physics/components/physics_enviroment.hpp>
#include <physics/events/events.hpp>
#include <physics/physx/physx_event_process_funcs.hpp>


namespace legion::physics
{
    using namespace physx;

    constexpr bool shouldDebugDraw = false;

    static void debugDrawPhysXScene(PxScene* sceneToDraw)
    {
        if (!shouldDebugDraw) return;

        const PxRenderBuffer& rb = sceneToDraw->getRenderBuffer();
        for (PxU32 i = 0; i < rb.getNbLines(); i++)
        {
            const PxDebugLine& line = rb.getLines()[i];

            const PxVec3& pxStart = line.pos0;
            const PxVec3& pxEnd = line.pos1;

            math::vec3 start{ pxStart.x, pxStart.y, pxStart.z };
            math::vec3 end{ pxEnd.x, pxEnd.y,pxEnd.z };

            debug::drawLine(start, end, math::colors::green, 1.0f, 0.0f);
        }
    }

    static void identifyPhysxDebugOutputFilepath(std::string& outFileName)
    {
        std::filesystem::path directoryPath = std::filesystem::current_path().parent_path().parent_path(); //leads to project directory
        directoryPath += "\\";

        std::time_t t = std::time(0);
        std::tm now;
        localtime_s(&now, &t);

        //file name is date and time of file creation
        std::string filename;
        filename += std::to_string(now.tm_year + 1900) + "-";
        filename += std::to_string(now.tm_mon + 1) + "-";
        filename += std::to_string(now.tm_mday)+ "-";
        filename += std::to_string(now.tm_hour) + "-";
        filename += std::to_string(now.tm_min) + "-";
        filename += std::to_string(now.tm_sec);
        filename += ".pxd2";

        outFileName = directoryPath.string();
        outFileName += filename;
    }

    //PhysX PVD Debugger Related
    constexpr const char* pvdHost = "127.0.0.1";
    constexpr size_type defaultPVDListeningPort = 5425;
    constexpr size_type defaultPVDHostTimeout = 10;

    class DebuggerWrapper
    {
    public:

        enum class transport_mode
        {
            pvd_network,
            file_output,
            none
        };

        void initializeDebugger(PxFoundation* foundation, transport_mode mode)
        {
            switch (mode)
            {
            case transport_mode::pvd_network:
                initializePVDDebugger(foundation);
                break;
            case transport_mode::file_output:
                initializeFileDebugger(foundation);
                break;
            case transport_mode::none:
                break;
            default:
                break;
            }
        }

        void release()
        {
            if (m_pvd)
            {
                m_pvd->disconnect();
                m_pvd->release();
                m_transport->release();
            }
        }

        PxPvd* getPVD() const noexcept { return m_pvd; }

    private:

        void initializeFileDebugger(PxFoundation* foundation)
        {
            if (!m_isDebuggerInit)
            {
                std::string filepath;
                identifyPhysxDebugOutputFilepath(filepath);
                log::debug("pth {0}", filepath.c_str());
                m_isDebuggerInit = true;
                m_pvd = PxCreatePvd(*foundation);
                m_transport = PxDefaultPvdFileTransportCreate(filepath.c_str());
                m_pvd->connect(*m_transport, PxPvdInstrumentationFlag::eALL);
            }
            else
            {
                log::warn("physx debugger already initialized!");
            }
        }

        void initializePVDDebugger(PxFoundation* foundation)
        {
            if (!m_isDebuggerInit)
            {
                m_isDebuggerInit = true;
                m_pvd = PxCreatePvd(*foundation);
                m_transport = PxDefaultPvdSocketTransportCreate(pvdHost, defaultPVDListeningPort, defaultPVDHostTimeout);
            }
            else
            {
                log::warn("physx debugger already initialized!");
            }
        }

        PxPvd* m_pvd = nullptr;
        PxPvdTransport* m_transport = nullptr;
        bool m_isDebuggerInit = false;
    };

    struct PhysxStatics
    {
        inline static size_type selfInstanceCounter = 0;

        inline static PxFoundation* foundation = nullptr;
        inline static DebuggerWrapper debugger;
        inline static PxDefaultCpuDispatcher* dispatcher = nullptr;
        inline static PxPhysics* physxSDK = nullptr;
        inline static PxCooking* cooking = nullptr;

        inline static PxDefaultAllocator defaultAllocator;
        inline static PxDefaultErrorCallback defaultErrorCallback;
        inline static async::spinlock setupShutdownLock;
    };

    using PS = PhysxStatics;

    constexpr size_type convexHullVertexLimit = 256;

    physx::PxPhysics* PhysXPhysicsSystem::getSDK()
    {
        return PS::physxSDK;
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
        bindToEvent<events::component_destruction<physics_component>, &PhysXPhysicsSystem::markPhysicsWrapperPendingRemove>();

        PhysicsComponentData::m_generateConvexColliderFunc = &PhysXPhysicsSystem::physxGenerateConvexMesh;

        //debugging related
        bindToEvent<request_flip_physics_continuous, &PhysXPhysicsSystem::flipPhysicsContinuousState>();
        bindToEvent<request_single_physics_tick, &PhysXPhysicsSystem::activateSingleStepContinue>();
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

    void* PhysXPhysicsSystem::physxGenerateConvexMesh(const std::vector<math::vec3>& vertices)
    {
        PxConvexMeshDesc convexDesc;
        convexDesc.points.count = vertices.size();
        convexDesc.points.stride = sizeof(math::vec3);
        convexDesc.points.data = vertices.data();
        convexDesc.flags =
            PxConvexFlag::eCOMPUTE_CONVEX | PxConvexFlag::eDISABLE_MESH_VALIDATION | PxConvexFlag::eFAST_INERTIA_COMPUTATION;
        convexDesc.vertexLimit = convexHullVertexLimit;

        PxDefaultMemoryOutputStream buf;
        if (!PS::cooking->cookConvexMesh(convexDesc, buf))
            return NULL;

        PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
        auto convex = PS::physxSDK->createConvexMesh(input);

        return convex;
    }

    void PhysXPhysicsSystem::lazyInitPhysXVariables()
    {
        if (PS::selfInstanceCounter == 1)
        {
            PS::foundation = PxCreateFoundation(PX_PHYSICS_VERSION,
                PS::defaultAllocator, PS::defaultErrorCallback);

            PS::dispatcher = PxDefaultCpuDispatcherCreate(0); //deal with multithreading later on

            #ifdef _DEBUG
            PS::debugger.initializeDebugger(PS::foundation, DebuggerWrapper::transport_mode::file_output);
            #endif

            PxTolerancesScale scale;
            PS::physxSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *PS::foundation, scale, true, PS::debugger.getPVD());

            PxCookingParams params(scale);
            params.meshWeldTolerance = 0.001f;
            params.meshPreprocessParams = PxMeshPreprocessingFlags(PxMeshPreprocessingFlag::eWELD_VERTICES);
            params.buildGPUData = false; //TODO set to true later on when GPU bodies are supported 

            PS::cooking = PxCreateCooking(PX_PHYSICS_VERSION, *PS::foundation, params);
        }
    }

    void PhysXPhysicsSystem::setupDefaultScene()
    {
        PxSceneDesc sceneDesc(PS::physxSDK->getTolerancesScale());
        sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
        sceneDesc.cpuDispatcher = PS::dispatcher;
        sceneDesc.filterShader = PxDefaultSimulationFilterShader;
        sceneDesc.flags |= PxSceneFlag::eENABLE_STABILIZATION;
        sceneDesc.flags |= PxSceneFlag::eENABLE_ACTIVE_ACTORS;

        m_physxScene = PS::physxSDK->createScene(sceneDesc);
        m_defaultMaterial = PS::physxSDK->createMaterial(0.5f, 0.5f, 0.6f);

        #ifdef _DEBUG
        m_physxScene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
        m_physxScene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
        m_physxScene->setVisualizationParameter(PxVisualizationParameter::eBODY_MASS_AXES, 1.0f);
        #endif

        PxPvdSceneClient* pvdClient = m_physxScene->getScenePvdClient();
        if (pvdClient)
        {
            pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
            pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
        }
    }

    void PhysXPhysicsSystem::bindEventsToEventProcessors()
    {
        m_physicsComponentActionFuncs[physics_component_flag::pc_add_first_box] = &processAddFirstBox;
        m_physicsComponentActionFuncs[physics_component_flag::pc_add_next_box] = &processAddNextBox;
        m_physicsComponentActionFuncs[physics_component_flag::pc_add_first_sphere] = &processAddFirstSphere;
        m_physicsComponentActionFuncs[physics_component_flag::pc_add_next_sphere] = &processAddNextSphere;
        m_physicsComponentActionFuncs[physics_component_flag::pc_add_first_convex] = &processAddFirstConvex;
        m_physicsComponentActionFuncs[physics_component_flag::pc_add_next_convex] = &processAddNextConvex;

        m_rigidbodyComponentActionFuncs[rigidbody_flag::rb_velocity] = &processVelocityModification;
        m_rigidbodyComponentActionFuncs[rigidbody_flag::rb_angular_velocity] = &processAngularVelocityModification;
        m_rigidbodyComponentActionFuncs[rigidbody_flag::rb_mass] = &processMassModification;
        m_rigidbodyComponentActionFuncs[rigidbody_flag::rb_angular_drag] = &processAngularDragModification;
        m_rigidbodyComponentActionFuncs[rigidbody_flag::rb_linear_drag] = &processLinearDragModification;

        m_enviromentComponentActionFuncs[physics_enviroment_flag::pe_add_plane] = &processAddInfinitePlane;
    }

    void PhysXPhysicsSystem::releasePhysXVariables()
    {
        PS::cooking->release();
        PS::dispatcher->release();
        PS::physxSDK->release();
        PS::debugger.release();
        PS::foundation->release();
    }

    void PhysXPhysicsSystem::update(legion::time::span deltaTime)
    {
        if (m_isContinuousStepActive || m_isSingleStepContinueAcitve)
        {
            m_accumulation += deltaTime;

            size_type tickAmount = 0;

            while (m_accumulation > m_timeStep && tickAmount < m_maxPhysicsStep)
            {
                m_accumulation -= m_timeStep;
                physicsStep();
                ++tickAmount;
            }

            m_isSingleStepContinueAcitve = false;
        }

        debugDrawPhysXScene(m_physxScene);
    }

    void PhysXPhysicsSystem::physicsStep()
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

        //[2] Identify new physics components and enviroments
        ecs::filter<physics_component> physicsComponentFilter;
        ecs::filter<physics_enviroment> physicsEnviromentFilter;

        for (auto entity : physicsComponentFilter)
        {
            auto& physComp = *entity.get_component<physics_component>();

            if (physComp.physicsComponentID == invalid_physics_component)
            {
                m_physxWrapperContainer.createPhysxWrapper(physComp);
            }
        }

        for (auto entity : physicsEnviromentFilter)
        {
            auto& physEnv = *entity.get_component<physics_enviroment>();

            if (physEnv.physicsEnviromentID == invalid_physics_enviroment)
            {
                m_physxWrapperContainer.createPhysxWrapper(physEnv);
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

        //[4] Go through events generated by physics_components,rigidbodies, and physics enviroments
        PhysxEnviromentInfo enviromentInfo;
        enviromentInfo.scene = m_physxScene;
        enviromentInfo.defaultMaterial = m_defaultMaterial;
        enviromentInfo.defaultRigidbodyDensity = 10.0f;

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

        for (auto entity : physicsEnviromentFilter)
        {
            auto& physEnv = *entity.get_component<physics_enviroment>();
            processPhysicsEnviromentEvents(entity, physEnv, enviromentInfo);
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

        //[2] Transfer linear and angular velocity changes from PxRigidDynamics to actual entity rigidbody components
        ecs::filter<physics_component, rigidbody> physicsAndRigidbodyComponentFilter;

        for (ecs::entity ent : physicsAndRigidbodyComponentFilter)
        {
            physics_component& phyComp = *ent.get_component<physics_component>();
            rigidbody& rbComp = *ent.get_component<rigidbody>();

            pointer<PhysxInternalWrapper> physxWrapper =  m_physxWrapperContainer.findWrapperWithID(phyComp.physicsComponentID);

            if (physxWrapper)
            {
                PxRigidDynamic* dynamic = static_cast<PxRigidDynamic*>(physxWrapper->physicsActor);

                PxVec3 pxLinear = dynamic->getLinearVelocity();
                PxVec3 pxAngular = dynamic->getAngularVelocity();

                rbComp.data.setLinearVelocityDirect({ pxLinear.x,pxLinear.y,pxLinear.z });
                rbComp.data.setAngularVelocityDirect({ pxAngular.x,pxAngular.y,pxAngular.z });
            }
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
        auto& eventsGenerated = rigidbody.data.getGeneratedModifyEvents();

        pointer<PhysxInternalWrapper> wrapperPtr = m_physxWrapperContainer.findWrapperWithID(physicsComponentID);

        if (!wrapperPtr) { return; }

        for (size_type bitPos = 0; bitPos < eventsGenerated.size(); ++bitPos)
        {
            if (eventsGenerated.test(bitPos))
            {
                m_rigidbodyComponentActionFuncs[bitPos].invoke(rigidbody, physicsEnviromentInfo, *wrapperPtr, ent);
            }
        }

        rigidbody.data.resetModificationFlags();
    }

    void PhysXPhysicsSystem::processPhysicsEnviromentEvents(ecs::entity ent, physics_enviroment& physicsComponentToProcess, const PhysxEnviromentInfo& physicsEnviromentInfo)
    {
        size_type enviromentID = physicsComponentToProcess.physicsEnviromentID;
        auto& eventsGenerated = physicsComponentToProcess.data.getGeneratedModifyEvents();

        pointer<PhysxInternalWrapper> wrapperPtr = m_physxWrapperContainer.findWrapperWithID(enviromentID);

        if (!wrapperPtr) { return; }

        for (size_type bitPos = 0; bitPos < eventsGenerated.size(); ++bitPos)
        {
            if (eventsGenerated.test(bitPos))
            {
                m_enviromentComponentActionFuncs[bitPos].invoke(physicsComponentToProcess, physicsEnviromentInfo, *wrapperPtr, ent);
            }
        }

        physicsComponentToProcess.data.resetModificationFlags();
    }
}
