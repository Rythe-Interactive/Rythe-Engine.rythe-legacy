#include "physx_test_system.hpp"
#include <rendering/components/camera.hpp>
#include <physics/physics.hpp>

namespace legion::physics
{
    constexpr size_t defaultSceneIndex = 0;

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

        setupCubeWorldTestScene();
    }

    void PhysXTestSystem::setupCubeWorldTestScene()
    {
        //add wide block
        auto wideBlock = createDefaultMeshEntity(math::vec3(0, 0, 0), cubeH, legionLogoMat);
        wideBlock.get_component<scale>() = math::vec3(10, 1, 10);

        {
            auto wideBlockPhysComp = wideBlock.add_component<physicsComponent>();
            wideBlockPhysComp->physicsCompData.AddBoxCollider(math::vec3(10, 1, 10));
        }

        //add default cube at center
        auto unrotatedBlock = createDefaultMeshEntity(math::vec3(0, 1, 0), cubeH, tileMat);
        {
            auto unrotatedBlockPC = unrotatedBlock.add_component<physicsComponent>();
            unrotatedBlockPC->physicsCompData.AddBoxCollider(math::vec3(1, 1, 1));
        }

        //add rotated default cube on top
        auto shiftedBlock = createDefaultMeshEntity(math::vec3(0, 10, 0), cubeH, concreteMat);
        {
            auto shiftedBlockPC = shiftedBlock.add_component<physicsComponent>();
            shiftedBlockPC->physicsCompData.AddBoxCollider(math::vec3(1, 1, 1));
        }

        //enable player to shoot blocks
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
        materialToInitialize.set_param("skycolor", math::color(0.1f, 0.3f, 1.0f));

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
}
