#include "physx_test_system.hpp"
#include <rendering/components/camera.hpp>
#include <physics/physics.hpp>
#include <random>

namespace legion::physics
{


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

        PhysicsHelpers::createPhysicsMaterial(0.5f, 0.5f, 0.1f, "DefaultNonBouncy");
        
        setupCubeWorldTestScene();
        //setupBoxAndStackTestScene();

        //enable player to shoot blocks
        app::InputSystem::createBinding<ShootPhysXBox>(app::inputmap::method::C);
        bindToEvent<ShootPhysXBox, &PhysXTestSystem::shootPhysXCubes>();

        app::InputSystem::createBinding<ShootPhysXSphere>(app::inputmap::method::V);
        bindToEvent<ShootPhysXSphere, &PhysXTestSystem::shootPhysXSphere>();
    }

    void PhysXTestSystem::update(legion::time::span deltaTime)
    {
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
            unrotatedBlockPC->physicsCompData.addBoxCollider(math::vec3(1));
            
            float headOffset = 0.5f;
            addRobotPartLambda(math::vec3(headOffset), math::vec3(0, 0.75f,0), unrotatedBlock,  math::identity<math::quat>(),cubeH);
            unrotatedBlockPC->physicsCompData.addBoxCollider(math::vec3(headOffset), math::vec3(0, 0.75f, 0), math::identity<math::quat>());

            addRobotPartLambda(math::vec3(1.0, 0.5, 0.5), math::vec3(1, 0.0f, 0), unrotatedBlock,  math::identity<math::quat>(), cubeH);
            unrotatedBlockPC->physicsCompData.addBoxCollider(math::vec3(1.0, 0.5f, 0.5f),math::vec3(1,0,0), math::identity<math::quat>());

            addRobotPartLambda(math::vec3(1.0, 0.5, 0.5), math::vec3(-1, 0.0f, 0), unrotatedBlock,  math::identity<math::quat>(), cubeH);
            unrotatedBlockPC->physicsCompData.addBoxCollider(math::vec3(1.0, 0.5f, 0.5f), math::vec3(-1, 0, 0), math::identity<math::quat>());

            addRobotPartLambda(math::vec3(1.0f), math::vec3(0, -1.0f, 0), unrotatedBlock, math::identity<math::quat>(),sphereH);
            unrotatedBlockPC->physicsCompData.addSphereCollider(0.5f, math::vec3(0, -1, 0));

            auto& rb = *unrotatedBlock.add_component<rigidbody>();
            rb.data.setMass(5.0f);
        }
 
        //add default cube on top
        auto shiftedBlock = createDefaultMeshEntity(math::vec3(0, 10, 0), cubeH, concreteMat);
        {
            auto shiftedBlockPC = shiftedBlock.add_component<physics_component>();
            shiftedBlockPC->physicsCompData.addBoxCollider(math::vec3(1, 1, 1));

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
            statuePhysicsComponent.physicsCompData.addConvexCollider(verts, math::vec3(), math::identity<math::quat>());
        }

        {
            auto suzanne = createDefaultMeshEntity(math::vec3(15, 8, 4), suzanneH, tileMat);
            auto& renderable = *suzanne.get_component<mesh_filter>();
            const std::vector<math::vec3>& verts = renderable.shared_mesh.ptr->vertices;

            auto& suzannePhysicsComponent = *suzanne.add_component<physics_component>();
            suzannePhysicsComponent.physicsCompData.addConvexCollider(verts, math::vec3(), math::identity<math::quat>());

            suzanne.add_component<rigidbody>();
        }
    }

    void PhysXTestSystem::setupBoxAndStackTestScene()
    {
        float stackZ = -10.0f;

        for (size_t i = 0; i < 20; i++)
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

    void PhysXTestSystem::shootPhysXCubes(ShootPhysXBox& action)
    {
        if (!action.value) { return; }

        math::vec3 cameraDirection; math::vec3 cameraPosition;
        getCameraPositionAndDirection(cameraDirection, cameraPosition);

        auto shiftedBlock = createDefaultMeshEntity(cameraPosition, cubeH, concreteMat);

        {
            auto shiftedBlockPC = shiftedBlock.add_component<physics_component>();
            shiftedBlockPC->physicsCompData.addBoxCollider(math::vec3(1, 1, 1));

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
            shiftedBlockPC->physicsCompData.addSphereCollider(0.5f, math::vec3());

            rigidbody& rb = *shiftedBlock.add_component<rigidbody>();
            rb.data.setVelocity(cameraDirection * 20.0f);
        }

        self_destruct_component& block = *shiftedBlock.add_component<self_destruct_component>();

        block.selfDestructTimer = 5.0f;
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
        phyComp.physicsCompData.addBoxCollider(scale);

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
            phyComp.physicsCompData.addConvexCollider(verts, math::vec3(), math::identity<math::quat>());
            phyComp.physicsCompData.getColliders()[0].setMaterialHash(hash);

            ent.add_component<rigidbody>();
        }

    }
    void PhysXTestSystem::createCubeStack(const math::vec3& extents, size_t stackSize, const math::vec3& startPos)
    {
        float floatStackSize = static_cast<float>(stackSize);

        for (size_type i = 0; i < stackSize; i++)
        {
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
