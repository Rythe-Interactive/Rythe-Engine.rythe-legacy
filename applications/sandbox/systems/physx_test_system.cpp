#include "physx_test_system.hpp"
#include <rendering/components/camera.hpp>
#include <physics/physics.hpp>
#include <random>

namespace legion::physics
{
    static ecs::entity triggerCharacterBlockDynamic;
    static ecs::entity triggerCharacterTriggerDynamic;

    static size_type glassMask = 0;
    static size_type bulletProofMask = 1;
    static size_type brickWallMask = 2;

    static void logQueryHit(QueryHit& logHit,const char* queryAction)
    {
        for (QueryHitInfo& overlapInfo : logHit.touchingHit)
        {
            log::debug("{0} overlapped: {1}", queryAction,overlapInfo.entityHit->name);
        }

        if (logHit.blockFound)
        {
            log::debug("{0} blocked by: {1}", queryAction, logHit.blockingHit.entityHit->name);
        }
        else
        {
            log::debug("{0} was not blocked by anything",queryAction);
        }
    }

    static void logOverlapHit(OverlapHit& hit)
    {
        if (hit.overlapHits.size() == 0)
        {
            log::debug("No Overlap!");
        }

        for (OverlapHitInfo& overlapInfo : hit.overlapHits)
        {
            log::debug("overlapped: {0}", overlapInfo.entityOverlapped->name);
        }
    }


    void PhysXTestSystem::setup()
    {
        using namespace legion::core::fs::literals;

        app::window& win = ecs::world.get_component<app::window>();
        app::context_guard guard(win);

        litShader = rendering::ShaderCache::create_shader("lit", fs::view("engine://shaders/default_lit.shs"));

        initializeLitMaterial(legionLogoMat, litShader,
            "assets://textures/split-test.png"_view, "assets://textures/tile/tileNormal.png"_view, "assets://textures/tile/tileRoughness.png"_view);

        initializeLitMaterial(tileMat, litShader,
            "assets://textures/tile/tileColor.png"_view, "assets://textures/tile/tileNormal.png"_view, "assets://textures/tile/tileRoughness.png"_view);

        initializeLitMaterial(concreteMat, litShader,
            "assets://textures/concrete.png"_view, "assets://textures/tile/tileNormal.png"_view, "assets://textures/tile/tileRoughness.png"_view);

        cubeH = rendering::ModelCache::create_model("cube", "assets://models/cube.obj"_view);
        sphereH = rendering::ModelCache::create_model("sphere", "assets://models/sphere.obj"_view);
        suzanneH = rendering::ModelCache::create_model("suzanne", "assets://models/suzanne.glb"_view);
        statueH = rendering::ModelCache::create_model("statue", "assets://models/gnomecentered.obj"_view);
        colaCanH = rendering::ModelCache::create_model("cola","assets://models/cola.glb"_view);

        directionalLightH = rendering::ModelCache::create_model("directional light", "assets://models/directional-light.obj"_view);

        auto lightshader = rendering::ShaderCache::create_shader("light", "assets://shaders/light.shs"_view);
        directionalLightMH = rendering::MaterialCache::create_material("directional light", lightshader);
        directionalLightMH.set_param("color", math::color(1, 1, 0.8f));
        directionalLightMH.set_param("intensity", 1.f);

        {
            auto sun = createEntity();
            sun.add_component<rendering::mesh_renderable>(mesh_filter(directionalLightH.get_mesh()), rendering::mesh_renderer(directionalLightMH));
            sun.add_component<rendering::light>(rendering::light::directional(math::color(1, 1, 0.8f), 10.f));
            sun.add_component<transform>(position(10, 10, 10), rotation::lookat(math::vec3(1, 1, -1), math::vec3::zero), scale());
        }

        PhysicsHelpers::createPhysicsMaterial(0.6f, 0.6f, 0.1f, "DefaultNonBouncy");
        PhysicsHelpers::createPhysicsMaterial(0.3f, 0.3f, 0.0f, "MediumNonBouncy");
        PhysicsHelpers::createPhysicsMaterial(0.1f, 0.1f, 0.0f, "SlipperyNonBouncy");
        
        
        //setupCubeWorldTestScene();
        //setupBoxAndStackTestScene();
        //setupCharacterControllerTestScene();
        //setupCollisionFilteringScene();
        setupSceneQueryScene();

        //enable player to shoot blocks
        app::InputSystem::createBinding<ShootPhysXBox>(app::inputmap::method::C);
        bindToEvent<ShootPhysXBox, &PhysXTestSystem::shootPhysXCubes>();

        app::InputSystem::createBinding<ShootPhysXSphere>(app::inputmap::method::V);
        bindToEvent<ShootPhysXSphere, &PhysXTestSystem::shootPhysXSphere>();

        app::InputSystem::createBinding<ShootFrictionAndForceCubes>(app::inputmap::method::B);
        bindToEvent<ShootFrictionAndForceCubes, &PhysXTestSystem::shootFrictionTest>();

        app::InputSystem::createBinding<CharacterJump>(app::inputmap::method::SPACE);
        bindToEvent<CharacterJump, &PhysXTestSystem::OnCharacterJump>();
    }

    void PhysXTestSystem::update(legion::time::span deltaTime)
    {
        float speed = 5.0f * deltaTime;

        if (moveBools[move_dir::forward])
        {
            MoveCharacter(math::vec3(0.0f, 0, speed) );
        }

        if (moveBools[move_dir::backward])
        {
            MoveCharacter(math::vec3(0.0f, 0, -speed) );
        }

        if (moveBools[move_dir::left])
        {
            MoveCharacter(math::vec3(-speed, 0,0 ) );
        }

        if (moveBools[move_dir::right])
        {
            MoveCharacter(math::vec3(speed, 0, 0) );
        }

        ecs::filter<self_destruct_component> destructFilter;

        for (auto entity : destructFilter)
        {
            self_destruct_component& sdComp = *entity.get_component<self_destruct_component>();
            sdComp.selfDestructTimer -= deltaTime;

            if (sdComp.selfDestructTimer < 0.0f)
            {
                entity.destroy();
            }
        }

        if (m_isRainingSuzanne)
        {
            suzzaneRainTick(deltaTime);
        }

        QueryMask weaponMask;
        weaponMask.setReactionToMaskIndex(glassMask, physics_object_reaction::reaction_overlap);
        weaponMask.setReactionToMaskIndex(bulletProofMask, physics_object_reaction::reaction_block);
        weaponMask.setReactionToMaskIndex(brickWallMask, physics_object_reaction::reaction_block);

        QueryMask visibilityMask;
        visibilityMask.setReactionToMaskIndex(glassMask, physics_object_reaction::reaction_overlap);
        visibilityMask.setReactionToMaskIndex(bulletProofMask, physics_object_reaction::reaction_overlap);
        visibilityMask.setReactionToMaskIndex(brickWallMask, physics_object_reaction::reaction_block);

        QueryMask overlapAllMask(physics_object_reaction::reaction_overlap);

        log::debug("------------------------- All Raycast Result: ---------------------------");

        QueryHit allRaycastHitInfo;
        PhysicsHelpers::raycast(math::vec3(15, 4, 5), math::vec3(0, 0, -1), 1000.0f, allRaycastHitInfo);

        logQueryHit(allRaycastHitInfo, "raycast");

        log::debug("------------------------- Weapon Raycast Result: ---------------------------");

        QueryHit weaponRaycastHitInfo;
        PhysicsHelpers::raycast(math::vec3(15, 3, 5), math::vec3(0, 0, -1), 1000.0f, weaponRaycastHitInfo, weaponMask);

        logQueryHit(weaponRaycastHitInfo,"raycast");

        log::debug("------------------------- Visibility Raycast Result: ---------------------------");

        QueryHit visibilityRaycastHitInfo;
        PhysicsHelpers::raycast(math::vec3(15, 2, 5), math::vec3(0, 0, -1), 1000.0f, visibilityRaycastHitInfo,visibilityMask);

        logQueryHit(visibilityRaycastHitInfo, "raycast");

        log::debug("------------------------- Overlap All Raycast Result: ---------------------------");

        QueryHit overlapRaycastHitInfo;
        PhysicsHelpers::raycast(math::vec3(15, 1, 5), math::vec3(0, 0, -1), 1000.0f, overlapRaycastHitInfo, overlapAllMask);

        logQueryHit(overlapRaycastHitInfo, "raycast");

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        log::debug("------------------------- All Sweep Result: ---------------------------");

        QueryHit allSweep;
        PhysicsHelpers::sweepSphere(1.0f, math::vec3(30, 10.0f, 5.0f), math::vec3(0, 0, -1), 30.0f, allSweep);

        logQueryHit(allSweep, "sweep");

        log::debug("------------------------- Weapon Sweep Result: ---------------------------");

        QueryHit weaponSweepInfo;
        PhysicsHelpers::sweepBox(math::vec3(0.5, 0.5, 0.5), math::vec3(30, 8.0f, 5.0f),
            math::identity<math::quat>(), math::vec3(0, 0, -1), 30.0f, weaponSweepInfo, weaponMask);

        logQueryHit(weaponSweepInfo, "sweep");

        log::debug("------------------------- Visibility Sweep Result: ---------------------------");

        QueryHit visibilitySweep;
        PhysicsHelpers::sweepSphere(1.0f, math::vec3(30, 6.0f, 5.0f), math::vec3(0, 0, -1), 30.0f, visibilitySweep,visibilityMask);

        logQueryHit(visibilitySweep, "sweep");


        log::debug("------------------------- Overlap All Sweep Result: ---------------------------");

        QueryHit overlapAllSweepInfo;
        PhysicsHelpers::sweepBox(math::vec3(0.5, 0.5, 0.5), math::vec3(30,4.0f, 5.0f),
            math::identity<math::quat>(), math::vec3(0, 0, -1), 30.0f, overlapAllSweepInfo, overlapAllMask);

        logQueryHit(overlapAllSweepInfo, "sweep");

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        log::debug("------------------------- Overlap Glass ---------------------------");

        QueryMask glassQuery;
        glassQuery.setReactionToMaskIndex(glassMask, physics_object_reaction::reaction_overlap);
        glassQuery.setReactionToDefaultMask(physics_object_reaction::reaction_ignore);

        OverlapHit glassOverlap;
        PhysicsHelpers::checkBoxOverlap(math::vec3(5, 5, 5), math::vec3(45.5, 0.5, 0), math::identity<math::quat>(),
            glassOverlap, glassQuery);

        logOverlapHit(glassOverlap);

       log::debug("------------------------- Overlap Bullet Proof ---------------------------");

       QueryMask bulletProofQuery;
       bulletProofQuery.setReactionToMaskIndex(bulletProofMask, physics_object_reaction::reaction_overlap);
       bulletProofQuery.setReactionToDefaultMask(physics_object_reaction::reaction_ignore);

       OverlapHit bulletProofOverlap;
       PhysicsHelpers::checkSphereOverlap(5, math::vec3(45.5, 0.5, 0), math::identity<math::quat>(),
           bulletProofOverlap, bulletProofQuery);

       logOverlapHit(bulletProofOverlap);

       log::debug("------------------------- Brick Wall Proof ---------------------------");

       QueryMask brickWallQuery;
       brickWallQuery.setReactionToMaskIndex(brickWallMask, physics_object_reaction::reaction_overlap);
       brickWallQuery.setReactionToDefaultMask(physics_object_reaction::reaction_ignore);

       OverlapHit brickWallOverlap;
       PhysicsHelpers::checkBoxOverlap(math::vec3(5, 5, 5), math::vec3(45.5, 0.5, 0), math::identity<math::quat>(),
           brickWallOverlap, brickWallQuery);

       logOverlapHit(brickWallOverlap);

       log::debug("------------------------- Overlap all ---------------------------");

       OverlapHit allOverlap;
       PhysicsHelpers::checkSphereOverlap(5, math::vec3(45.5, 0.5, 0), math::identity<math::quat>(),
           allOverlap, overlapAllMask);

       logOverlapHit(allOverlap);

       /* std::cout << "---------------------- overlap with mask 0 ----------------------" << std::endl;

        PxU32 oneMask = setupRaycastMask(gOverlapOneMask);
        overlap(PxBoxGeometry(5.0f, 5.0f, 5.0f), PxVec3(45.5f, 0.5f, 0), oneMask);

        std::cout << "---------------------- overlap with mask 1 ----------------------" << std::endl;

        PxU32 twoMask = setupRaycastMask(gOverlapTwoMask);
        overlap(PxSphereGeometry(5.0f), PxVec3(45.5f, 0.5f, 0), twoMask);

        std::cout << "---------------------- overlap with mask 2 ----------------------" << std::endl;

        PxU32 threeMask = setupRaycastMask(gOverlapThreeMask);
        overlap(PxBoxGeometry(5.0f, 5.0f, 5.0f), PxVec3(45.5f, 0.5f, 0), threeMask);

        std::cout << "---------------------- overlap all ----------------------" << std::endl;

        PxU32 allOverlapMask = setupRaycastMask(gOverlapThreeMask, true);
        overlap(PxBoxGeometry(5.0f, 5.0f, 5.0f), PxVec3(45.5f, 0.5f, 0), allOverlapMask);*/
    }

    void PhysXTestSystem::setupCubeWorldTestScene()
    {
        math::quat rotX90 = math::rotate(math::pi<float>() / 2.0f, math::vec3(1, 0, 0));
        math::quat rotY90 = math::rotate(math::pi<float>() / 2.0f, math::vec3(0, 1, 0));
        math::quat rotZ90 = math::rotate(math::pi<float>() / 2.0f, math::vec3(0, 0, 1));

        auto addRobotPartLambda = [this](const math::vec3& scaleValue, const math::vec3& offset,ecs::entity parent,const math::quat& localRot,rendering::model_handle handle)
        {
            auto nextBlock = createDefaultMeshEntity(offset, handle, legionLogoMat);
            nextBlock.get_component<scale>() = scaleValue;
            nextBlock.get_component<rotation>() = localRot;

            parent.add_child(nextBlock);
        };

        createStaticColliderWall(math::vec3(0, 0, 5), tileMat, math::vec3(10, 1, 10),
            rotX90);

        createStaticColliderWall(math::vec3(0, 0, 0), legionLogoMat, math::vec3(10, 1, 10));

        //add default cube at center
        auto unrotatedBlock = createDefaultMeshEntity(math::vec3(0, 2, 0), cubeH, tileMat);
        {
            auto unrotatedBlockPC = unrotatedBlock.add_component<physics_component>();
            unrotatedBlockPC->physicsCompData.AddBoxCollider(math::vec3(1));
            
            float headOffset = 0.5f;
            addRobotPartLambda(math::vec3(headOffset), math::vec3(0, 0.75f,0), unrotatedBlock,  math::identity<math::quat>(),cubeH);
            unrotatedBlockPC->physicsCompData.AddBoxCollider(math::vec3(headOffset), math::vec3(0, 0.75f, 0), math::identity<math::quat>());

            addRobotPartLambda(math::vec3(1.0, 0.5, 0.5), math::vec3(1, 0.0f, 0), unrotatedBlock,  math::identity<math::quat>(), cubeH);
            unrotatedBlockPC->physicsCompData.AddBoxCollider(math::vec3(1.0, 0.5f, 0.5f),math::vec3(1,0,0), math::identity<math::quat>());

            addRobotPartLambda(math::vec3(1.0, 0.5, 0.5), math::vec3(-1, 0.0f, 0), unrotatedBlock,  math::identity<math::quat>(), cubeH);
            unrotatedBlockPC->physicsCompData.AddBoxCollider(math::vec3(1.0, 0.5f, 0.5f), math::vec3(-1, 0, 0), math::identity<math::quat>());

            addRobotPartLambda(math::vec3(1.0f), math::vec3(0, -1.0f, 0), unrotatedBlock, math::identity<math::quat>(),sphereH);
            unrotatedBlockPC->physicsCompData.AddSphereCollider(0.5f, math::vec3(0, -1, 0));

            auto& rb = *unrotatedBlock.add_component<rigidbody>();
            rb.data.setMass(5.0f);
        }
 
        //add default cube on top
        auto shiftedBlock = createDefaultMeshEntity(math::vec3(0, 10, 0), cubeH, concreteMat);
        {
            auto shiftedBlockPC = shiftedBlock.add_component<physics_component>();
            shiftedBlockPC->physicsCompData.AddBoxCollider(math::vec3(1, 1, 1));

            shiftedBlock.add_component<rigidbody>();
        }
        //---------------------------------------------- CONVEX TEST -----------------------------------------------------//

        m_isRainingSuzanne = true;

        createStaticColliderWall(math::vec3(15, 0, 0), legionLogoMat, math::vec3(10, 1, 10));

        createStaticColliderWall(math::vec3(15, 0, 5), tileMat, math::vec3(10, 1, 10), rotX90);

        createStaticColliderWall(math::vec3(10, 0, 0), tileMat, math::vec3(10, 1, 10), rotZ90);

        createStaticColliderWall(math::vec3(20, 0, 0), tileMat, math::vec3(10, 1, 10), rotZ90);
      
        {
            auto statue = createDefaultMeshEntity(math::vec3(15, 1.6f, 0), statueH, concreteMat);
            *statue.get_component<rotation>() = math::rotate(math::pi<float>(), math::vec3(0, 1, 0));
            auto& renderable = *statue.get_component<mesh_filter>();
            const std::vector<math::vec3>& verts = renderable.shared_mesh.ptr->vertices;

            auto& statuePhysicsComponent = *statue.add_component<physics_component>();
            statuePhysicsComponent.physicsCompData.AddConvexCollider(verts, math::vec3(), math::identity<math::quat>());
        }

        {
            auto suzanne = createDefaultMeshEntity(math::vec3(15, 8, 4), suzanneH, tileMat);
            auto& renderable = *suzanne.get_component<mesh_filter>();
            const std::vector<math::vec3>& verts = renderable.shared_mesh.ptr->vertices;

            auto& suzannePhysicsComponent = *suzanne.add_component<physics_component>();
            suzannePhysicsComponent.physicsCompData.AddConvexCollider(verts, math::vec3(), math::identity<math::quat>());

            suzanne.add_component<rigidbody>();
        }

        //---------------------------------------------- Friction Test TEST -----------------------------------------------------//

        auto ent = createStaticColliderWall(math::vec3(-15, 0.0f, 10.0f), legionLogoMat, math::vec3(10, 1, 40));

    }

    void PhysXTestSystem::setupBoxAndStackTestScene()
    {
        float stackZ = -10.0f;

        for (size_t i = 0; i < 10; i++)
        {
            createCubeStack(math::vec3(2.0f), 20, math::vec3(0,0, stackZ -= 10.0f));
        }
        
        ecs::entity bigSphere = createDefaultMeshEntity(math::vec3(0, 20, 100), sphereH, concreteMat);
        *bigSphere.add_component<scale>() = math::vec3(5.0f);
        bigSphere.add_component<physics_component>()->physicsCompData.AddSphereCollider(5.0f, math::vec3());

        auto& rbData = (*bigSphere.add_component<rigidbody>()).data;
        rbData.setVelocity(math::vec3(0, -25, -100));
        rbData.setMass(525398.719f);
        rbData.setAngularDrag(0.5f);

        ecs::entity groundPlane = createEntity();
        groundPlane.add_component<physics_enviroment>()->data.instantiateInfinitePlane(math::vec3(0,1,0),0.0f);
    }

    void PhysXTestSystem::setupCharacterControllerTestScene()
    {
        math::quat rotZ90 = math::rotate(math::pi<float>() / 2.0f, math::vec3(0, 0, 1));

        math::quat rotY30 = math::rotate(math::pi<float>() / 2.0f, math::vec3(0, 0, 1));

        m_characterControllerEnt = createDefaultMeshEntity(math::vec3(3.0f, 3.8f, -15.0f), sphereH, concreteMat);
        
        auto& capsuleCont = *m_characterControllerEnt.add_component<capsule_controller>();
        CapsuleControllerData& capsule = capsuleCont.data;

        gravity_preset gravity;
        gravity.gravityValue = math::vec3(0.0f, -0.98f, 0.0f);
        //gravity.gravityAcc = math::vec3(0.0f);
        capsule.addPreset(gravity);

        rigidbody_force_feedback force_feedback;
        force_feedback.forceAmount = 1000.0f;
        force_feedback.massMaximum = 50.0f;

        capsule.addPreset(force_feedback);

        capsule.setHeight(2.0f);
        capsule.setRadius(1.0f);
        capsule.setSpeed(0.1f);

        auto ent = createStaticColliderWall(math::vec3(0, 0.0f, 0), legionLogoMat, math::vec3(20, 1, 40));

        auto entFinalWall = createStaticColliderWall(math::vec3(0, 0.0f, 40.0f), legionLogoMat, math::vec3(20, 1, 20));

        auto blockRotLeft = createStaticColliderWall(math::vec3(2.5f, 1.0f, 0), legionLogoMat, math::vec3(5, 1, 1));
        *blockRotLeft.get_component<rotation>() = math::rotate(math::pi<float>() / 6.0f, math::vec3(0, 1, 0));

        auto blockRotRight = createStaticColliderWall(math::vec3(-2.5f, 1.0f, 5.0f), legionLogoMat, math::vec3(5, 1, 1));
        *blockRotRight.get_component<rotation>() = math::rotate(math::pi<float>() / 6.0f,  math::vec3(0, -1, 0));

        auto blockCenter = createStaticColliderWall(math::vec3(0.0f, 1.0f, 10.0f), legionLogoMat, math::vec3(5, 1, 1));
        //blockCenter->name = "center";

        auto inclineHeight = createStaticColliderWall(math::vec3(0, 3.0f, 25.0f), tileMat, math::vec3(20, 1, 15));
        *inclineHeight.get_component<rotation>() = math::rotate(math::pi<float>() / 6.0f, math::vec3(-1, 0, 0));

        createCubeStack(math::vec3(3.0f,1.0f,2.0f), 2, math::vec3(4.75f, 0.5f, -10.0f),1);

        setupCharacterMoveBindings();
    }

    void PhysXTestSystem::setupCollisionFilteringScene()
    {
        //setup character controller
        m_characterControllerEnt = createDefaultMeshEntity({ 0,2.0,10 }, sphereH, concreteMat);
        m_characterControllerEnt->name = "controller";

        auto& capsuleCont = *m_characterControllerEnt.add_component<capsule_controller>();
        CapsuleControllerData& capsule = capsuleCont.data;

        gravity_preset gravity;
        gravity.gravityValue = math::vec3(0.0f, -0.98f, 0.0f);
        //gravity.gravityAcc = math::vec3(0.0f);
        capsule.addPreset(gravity);

        rigidbody_force_feedback force_feedback;
        force_feedback.forceAmount = 1000.0f;
        force_feedback.massMaximum = 50.0f;

        capsule.addPreset(force_feedback);

        capsule.setHeight(1.5f);
        capsule.setRadius(0.45f);
        capsule.setSpeed(0.1f);

        //setup ground plane
        auto groundPlane = createEntity(); groundPlane->name = "plane";
        auto& enviromentComp = *groundPlane.add_component<physics_enviroment>();
        enviromentComp.data.instantiateInfinitePlane(math::vec3(0, 1, 0), 0);

        auto scaleAndAddPhysicsComp = [](ecs::entity ent)->physics_component&
        {
            auto& physicsComp = *ent.add_component<physics_component>();
            physicsComp.physicsCompData.AddBoxCollider({ 5.0f,1.0f,1.0f });
            *ent.get_component<scale>() = math::vec3{ 5.0f,1.0f,1.0f };

            return physicsComp;
        };

        //setup invisible wall
        triggerCharacterBlockDynamic = createDefaultMeshEntity({ 0, 0.5f, 0 }, cubeH, concreteMat); triggerCharacterBlockDynamic->name = "firstWall";
        physics_component& invisibleWall = scaleAndAddPhysicsComp(triggerCharacterBlockDynamic);

        invisibleWall.physicsCompData.setAllColliderReactionToObject(physics_object_flag::po_character_controller, physics_object_reaction::reaction_overlap);

        //setup trigger wall
        triggerCharacterTriggerDynamic = createDefaultMeshEntity({ 0, 0.5f, -5 }, cubeH, concreteMat); triggerCharacterTriggerDynamic->name = "triggerWall";
        physics_component& triggerWall = scaleAndAddPhysicsComp(triggerCharacterTriggerDynamic);

        triggerWall.physicsCompData.setAllColliderReactionToObject(physics_object_flag::po_character_controller, physics_object_reaction::reaction_overlap);
        triggerWall.physicsCompData.setAllColliderReactionToObject(physics_object_flag::po_dynamic, physics_object_reaction::reaction_overlap);

        //setup third block
        auto blockCharacterBlockDynamic = createDefaultMeshEntity({ 0, 0.5f, -10 }, cubeH, concreteMat); blockCharacterBlockDynamic->name = "thirdWall";
        physics_component& thirdBlock = scaleAndAddPhysicsComp(blockCharacterBlockDynamic);

        setupCharacterMoveBindings();

        bindToEvent<on_trigger_enter, &PhysXTestSystem::triggerEnterEvent>();
        bindToEvent<on_trigger_exit, &PhysXTestSystem::triggerExitEvent>();
    }

    void PhysXTestSystem::setupSceneQueryScene()
    {
        //create four walls for raycast

        auto glassWall = createStaticColliderWall(math::vec3(15, 3, 0), legionLogoMat, math::vec3(4.0f, 6.0f, 2.0f));
        glassWall.get_component<physics_component>()->physicsCompData.setAllColliderMask(glassMask);
        glassWall->name = "glassWall";
       
        auto bulletProofWall = createStaticColliderWall(math::vec3(15, 3, -5), legionLogoMat, math::vec3(4.0f, 6.0f, 2.0f));
        bulletProofWall.get_component<physics_component>()->physicsCompData.setAllColliderMask(bulletProofMask);
        bulletProofWall->name = "bulletProofWall";

        auto brickWall = createStaticColliderWall(math::vec3(15, 3, -10), legionLogoMat, math::vec3(4.0f, 6.0f, 2.0f));
        brickWall.get_component<physics_component>()->physicsCompData.setAllColliderMask(brickWallMask);
        brickWall->name = "brickWall";

        auto finalBrickWall = createStaticColliderWall(math::vec3(15, 3, -15), legionLogoMat, math::vec3(4.0f, 6.0f, 2.0f));
        finalBrickWall->name = "finalBrickWall";

        //create four walls for sweep

        auto glassWallSweep = createStaticColliderWall(math::vec3(30, 6, 0), legionLogoMat, math::vec3(8.0f, 12.0f, 2.0f));
        glassWallSweep.get_component<physics_component>()->physicsCompData.setAllColliderMask(glassMask);
        glassWallSweep->name = "glassWallSweep";

        auto bulletProofWallSweep = createStaticColliderWall(math::vec3(30, 6, -5), legionLogoMat, math::vec3(8.0f, 12.0f, 2.0f));
        bulletProofWallSweep.get_component<physics_component>()->physicsCompData.setAllColliderMask(bulletProofMask);
        bulletProofWallSweep->name = "bulletProofWallSweep";

        auto brickWallSweep = createStaticColliderWall(math::vec3(30, 6, -10), legionLogoMat, math::vec3(8.0f, 12.0f, 2.0f));
        brickWallSweep.get_component<physics_component>()->physicsCompData.setAllColliderMask(brickWallMask);
        brickWallSweep->name = "brickWallSweep";

        auto finalBrickWallSweep = createStaticColliderWall(math::vec3(30, 6, -15), legionLogoMat, math::vec3(8.0f, 12.0f, 2.0f));
        finalBrickWallSweep->name = "finalBrickWallSweep";

        //create boxes for overlap
        auto box1 = createStaticColliderWall(math::vec3(45, 0.5f, 0), legionLogoMat, math::vec3(1.0f, 1.0f, 1.0f));
        box1.get_component<physics_component>()->physicsCompData.setAllColliderMask(glassMask);
        box1->name = "box1";

        auto box2 = createStaticColliderWall(math::vec3(46, 0.5f, 0), legionLogoMat, math::vec3(1.0f, 1.0f, 1.0f));
        box2.get_component<physics_component>()->physicsCompData.setAllColliderMask(bulletProofMask);
        box2->name = "box2";

        auto box3 = createStaticColliderWall(math::vec3(45.5, 1.5f, 0), legionLogoMat, math::vec3(1.0f, 1.0f, 1.0f));
        box3.get_component<physics_component>()->physicsCompData.setAllColliderMask(brickWallMask);
        box3->name = "box3";
        
    }

    void PhysXTestSystem::shootPhysXCubes(ShootPhysXBox& action)
    {
        if (!action.value) { return; }

        math::vec3 cameraDirection; math::vec3 cameraPosition;
        getCameraPositionAndDirection(cameraDirection, cameraPosition);

        auto shiftedBlock = createDefaultMeshEntity(cameraPosition, cubeH, concreteMat);

        {
            auto shiftedBlockPC = shiftedBlock.add_component<physics_component>();
            shiftedBlockPC->physicsCompData.AddBoxCollider(math::vec3(1, 1, 1));

            rigidbody& rb = *shiftedBlock.add_component<rigidbody>();
            rb.data.setVelocity(cameraDirection * 20.0f);
            rb.data.setLinearDrag(0.5f);
        }

        self_destruct_component& block = *shiftedBlock.add_component<self_destruct_component>();

        block.selfDestructTimer = 5.0f;
    }

    void PhysXTestSystem::shootPhysXSphere(ShootPhysXSphere& action)
    {
        if (!action.value) { return; }

        math::vec3 cameraDirection; math::vec3 cameraPosition;
        getCameraPositionAndDirection(cameraDirection, cameraPosition);

        auto shiftedBlock = createDefaultMeshEntity(cameraPosition, sphereH, concreteMat);

        {
            auto shiftedBlockPC = shiftedBlock.add_component<physics_component>();
            shiftedBlockPC->physicsCompData.AddSphereCollider(0.5f, math::vec3());

            rigidbody& rb = *shiftedBlock.add_component<rigidbody>();
            rb.data.setVelocity(cameraDirection * 20.0f);
        }

        self_destruct_component& block = *shiftedBlock.add_component<self_destruct_component>();

        block.selfDestructTimer = 5.0f;
    }

    void PhysXTestSystem::shootFrictionTest(ShootFrictionAndForceCubes& action)
    {
        if (!action.value) { return; }

        size_type slip = PhysicsHelpers::retrievePhysicsMaterialHash("SlipperyNonBouncy");
        size_type medium = PhysicsHelpers::retrievePhysicsMaterialHash("MediumNonBouncy");
        size_type normal = PhysicsHelpers::retrievePhysicsMaterialHash("DefaultNonBouncy");

        {
            auto middle = createDefaultMeshEntity(math::vec3(-12, 1.0f, -5.f), cubeH, concreteMat);

            auto& phyComp = *middle.add_component<physics_component>();
            phyComp.physicsCompData.AddBoxCollider(math::vec3(1.0f));
            phyComp.physicsCompData.getColliders()[0].setMaterialHash(slip);
            middle.add_component<rigidbody>()->data.setVelocity(math::vec3(0,0,10.f));
        }

        {
            auto middle = createDefaultMeshEntity(math::vec3(-15, 1.0f, -5.f), cubeH, concreteMat);

            auto& phyComp = *middle.add_component<physics_component>();
            phyComp.physicsCompData.AddBoxCollider(math::vec3(1.0f));
            phyComp.physicsCompData.getColliders()[0].setMaterialHash(medium);

            middle.add_component<rigidbody>()->data.setVelocity(math::vec3(0, 0, 10.f));
        }

        {
            auto middle = createDefaultMeshEntity(math::vec3(-17, 1.0f, -5.f), cubeH, concreteMat);

            auto& phyComp = *middle.add_component<physics_component>();
            phyComp.physicsCompData.AddBoxCollider(math::vec3(1.0f));
            phyComp.physicsCompData.getColliders()[0].setMaterialHash(normal);

            middle.add_component<rigidbody>()->data.setVelocity(math::vec3(0, 0, 10.f));
        }
    }

    void PhysXTestSystem::getCameraPositionAndDirection(math::vec3& cameraDirection, math::vec3& cameraPosition)
    {
        //get camera position and set transform to camera postiion 
        ecs::filter<rendering::camera> cameraQuery;

        //assume the first camera is the player controlled camera
        for (auto ent : cameraQuery)
        {
            auto [positionCamH, rotationCamH, scaleCamH] = ent.get_component<transform>();
            cameraDirection = rotationCamH.get() * math::vec3(0, 0, 1);
            cameraPosition = positionCamH.get() + cameraDirection;
        }
    }

    void PhysXTestSystem::setupCharacterMoveBindings()
    {
        app::InputSystem::createBinding<MoveForward>(app::inputmap::method::T);
        bindToEvent<MoveForward, &PhysXTestSystem::onPressForward>();

        app::InputSystem::createBinding<MoveLeft>(app::inputmap::method::F);
        bindToEvent<MoveLeft, &PhysXTestSystem::onPressLeft>();

        app::InputSystem::createBinding<MoveRight>(app::inputmap::method::H);
        bindToEvent<MoveRight, &PhysXTestSystem::onPressRight>();

        app::InputSystem::createBinding<MoveBackward>(app::inputmap::method::G);
        bindToEvent<MoveBackward, &PhysXTestSystem::onPresBackward>();
    }

    void PhysXTestSystem::MoveCharacter(const math::vec3& displacement)
    {
        if (!m_characterControllerEnt.valid()) return;
        CapsuleControllerData& data =  m_characterControllerEnt.get_component<capsule_controller>()->data;
        data.moveTo(displacement);
    }

    void PhysXTestSystem::OnCharacterJump(CharacterJump& action)
    {
        if (!m_characterControllerEnt.valid()) return;

        if (action.value)
        {
            CapsuleControllerData& data = m_characterControllerEnt.get_component<capsule_controller>()->data;
            data.jump(math::vec3(0, 0.30f, 0));
        }
    }

    void PhysXTestSystem::triggerEnterEvent(on_trigger_enter& triggerEnter)
    {
        ecs::entity first = triggerEnter.collision.firstEntity;
        ecs::entity second = triggerEnter.collision.secondEntity;

        if (first == m_characterControllerEnt && second == triggerCharacterBlockDynamic)
        {
            auto left = createDefaultMeshEntity({ -2, 10.f, -5 }, sphereH, concreteMat);
            left.add_component<physics_component>()->physicsCompData.AddSphereCollider(0.5f,{});
            left.add_component<rigidbody>();


            auto right =createDefaultMeshEntity({ 2, 10.f, -5 }, sphereH, concreteMat);
            right.add_component<physics_component>()->physicsCompData.AddSphereCollider(0.5f, {});
            right.add_component<rigidbody>();
        }

        bool firstHasRB = first.has_component<rigidbody>();
        bool secondHasRB = second.has_component<rigidbody>();

        static bool firstTimeTrigger = true;

        bool colWithTrigger = first == triggerCharacterTriggerDynamic || second == triggerCharacterTriggerDynamic;
        bool colWithRB = (firstHasRB || secondHasRB);
        if (colWithTrigger && colWithRB && firstTimeTrigger)
        {
            firstTimeTrigger = false;
            {
                auto left = createDefaultMeshEntity({ -2, 10.f, 0 }, sphereH, concreteMat);
                left.add_component<physics_component>()->physicsCompData.AddSphereCollider(0.5f, {});
                left.add_component<rigidbody>();


                auto right = createDefaultMeshEntity({ 2, 10.f, 0 }, sphereH, concreteMat);
                right.add_component<physics_component>()->physicsCompData.AddSphereCollider(0.5f, {});
                right.add_component<rigidbody>();
            }

            {
                auto left = createDefaultMeshEntity({ -2, 10.f, -10 }, sphereH, concreteMat);
                left.add_component<physics_component>()->physicsCompData.AddSphereCollider(0.5f, {});
                left.add_component<rigidbody>();


                auto right = createDefaultMeshEntity({ 2, 10.f, -10 }, sphereH, concreteMat);
                right.add_component<physics_component>()->physicsCompData.AddSphereCollider(0.5f, {});
                right.add_component<rigidbody>();
            }
        }

    }

    void PhysXTestSystem::triggerExitEvent(on_trigger_exit& triggerExit)
    {

    }

    void PhysXTestSystem::initializeLitMaterial(rendering::material_handle& materialToInitialize,
        rendering::shader_handle& litShader, const fs::view& albedoFile, const fs::view& normalFile, const fs::view& roughnessFile)
    {
        static size_t litCount = 0;

        materialToInitialize = rendering::MaterialCache::create_material("LitMaterial" + litCount, litShader);
        materialToInitialize.set_param("alphaCutoff", 0.5f);
        materialToInitialize.set_param("useAlbedoTex", true);
        materialToInitialize.set_param("useRoughnessTex", true);
        materialToInitialize.set_param("useNormal", true);

        materialToInitialize.set_param("useEmissiveTex", false);
        materialToInitialize.set_param("useAmbientOcclusion", false);
        materialToInitialize.set_param("useHeight", false);
        materialToInitialize.set_param("useMetallicTex", false);
        materialToInitialize.set_param("useMetallicRoughness", false);

        materialToInitialize.set_param("metallicValue", 0.0f);
        materialToInitialize.set_param("emissiveColor", math::colors::black);

        materialToInitialize.set_param("albedoTex", rendering::TextureCache::create_texture(albedoFile));
        materialToInitialize.set_param("normalTex", rendering::TextureCache::create_texture(normalFile));
        materialToInitialize.set_param("roughnessTex", rendering::TextureCache::create_texture(roughnessFile));

        litCount++;
    }

    ecs::entity PhysXTestSystem::createDefaultMeshEntity(math::vec3 position, rendering::model_handle cubeH, rendering::material_handle TextureH)
    {
        auto ent = createEntity();

        auto [positionH, rotationH, scaleH] = ent.add_component<transform>();
        positionH = position;

        ent.add_component(rendering::mesh_renderer(TextureH, cubeH));

        return ent;
    }

    ecs::entity PhysXTestSystem::createStaticColliderWall(math::vec3 position, rendering::material_handle TextureH,
        math::vec3 scale, math::quat rot)
    {
        auto ent = createEntity();

        auto [positionH, rotationH, scaleH] = ent.add_component<transform>();
        ent.add_component(rendering::mesh_renderer(TextureH, cubeH));

        positionH = position;
        rotationH = rot;
        scaleH = scale;

        auto& phyComp = *ent.add_component<physics_component>();
        phyComp.physicsCompData.AddBoxCollider(scale);

        return ent;
    }

    void PhysXTestSystem::suzzaneRainTick(legion::time::span deltaTime)
    {
        size_type hash = PhysicsHelpers::retrievePhysicsMaterialHash("DefaultNonBouncy");

        m_currentInterval += deltaTime;

        if (m_currentInterval > m_rainInterval)
        {
            std::array<gfx::model_handle, 3> models{ colaCanH,statueH,suzanneH };

            m_currentInterval = 0.0f;

            static std::mt19937 generator;
            std::uniform_real_distribution<double> xGen(0,m_rainExtents.x);
            std::uniform_real_distribution<double> yGen(0, m_rainExtents.y);
            std::uniform_real_distribution<double> zGen(0, m_rainExtents.z);

            std::uniform_int_distribution<std::mt19937::result_type> modelGen(0, 2);

            math::vec3 randomPosition = m_rainStartPos + math::vec3(xGen(generator), yGen(generator), zGen(generator));

            auto ent = createDefaultMeshEntity(randomPosition, models[modelGen(generator)], tileMat);
            auto& renderable = *ent.get_component<mesh_filter>();
            const std::vector<math::vec3>& verts = renderable.shared_mesh.ptr->vertices;

            auto& phyComp = *ent.add_component<physics_component>();
            phyComp.physicsCompData.AddConvexCollider(verts, math::vec3(), math::identity<math::quat>());
            phyComp.physicsCompData.getColliders()[0].setMaterialHash(hash);

            ent.add_component<rigidbody>();
        }

    }

    void PhysXTestSystem::createCubeStack(const math::vec3& extents, size_t stackSize, const math::vec3& startPos, int stopAfterStack)
    {
        float floatStackSize = static_cast<float>(stackSize);

        for (size_type i = 0; i < stackSize; i++)
        {
            if (i == stopAfterStack) { return; }

            for (size_type j = 0; j < floatStackSize - i; j++)
            {
                //PxTransform localTm(PxVec3(PxReal(j*2) - PxReal(size-i), PxReal(i*2+1), 0) * halfExtent);
                
                math::vec3 offset = math::vec3(j * 2 - (floatStackSize - i), (i * 2) + 1, 0) * (extents * 0.5f);

                auto entity = createDefaultMeshEntity(startPos + offset, cubeH, legionLogoMat);
                *entity.get_component<scale>() = extents;

                auto& physComp = *entity.add_component<physics_component>();
                physComp.physicsCompData.AddBoxCollider(extents);

                entity.add_component<rigidbody>();
            }
        }
    }
}
