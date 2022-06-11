#include <chrono>

#include "../systems/examplesystem.hpp"
#include "../defaults/defaultpolicies.hpp"
#include <rendering/data/particlepolicies/renderpolicy.hpp>
#include <rendering/data/particlepolicies/flipbookpolicy.hpp>


void ExampleSystem::setup()
{
    using namespace legion;
    log::filter(log::severity_debug);
    log::debug("ExampleSystem setup");

    //auto* pipeline = dynamic_cast<gfx::DefaultPipeline*>(gfx::Renderer::getMainPipeline());
    //if (pipeline)
    //    pipeline->attachStage<MouseHover>();

    app::window& win = ecs::world.get_component<app::window>();
    app::context_guard guard(win);

    auto model = gfx::ModelCache::create_model("Sphere", fs::view("assets://models/sphere.obj"));
    gfx::ModelCache::create_model("ParticleGizmo", fs::view("assets://models/sphere.obj"));

    auto material = gfx::MaterialCache::create_material("White", fs::view("assets://shaders/color.shs"));
    material.set_param("color", math::colors::white);

    //Iron
    {
        /* material = gfx::MaterialCache::create_material("iron", fs::view("engine://shaders/default_lit.shs"));

         material.set_param("alphaCutoff", 0.5f);
         material.set_param("useAlbedoTex", true);
         material.set_param("useRoughnessTex", true);
         material.set_param("useNormal", true);
         material.set_param("useAmbientOcclusion", true);
         material.set_param("useHeight", true);
         material.set_param("useMetallicTex", true);

         material.set_param("useEmissiveTex", false);
         material.set_param("useMetallicRoughness", false);

         material.set_param("ambientOcclusionTex", rendering::TextureCache::create_texture(fs::view("assets://textures/iron/rustediron-ao.png")));
         material.set_param("heightTex", rendering::TextureCache::create_texture(fs::view("assets://textures/iron/rustediron-height.png")));
         material.set_param("heightScale", 1.f);
         material.set_param("metallicTex", rendering::TextureCache::create_texture(fs::view("assets://textures/iron/rustediron-metallic.png")));
         material.set_param("emissiveColor", math::colors::black);

         material.set_param("albedoTex", rendering::TextureCache::create_texture(fs::view("assets://textures/iron/rustediron-albedo-2048.png")));
         material.set_param("normalTex", rendering::TextureCache::create_texture(fs::view("assets://textures/iron/rustediron-normal.png")));
         material.set_param("roughnessTex", rendering::TextureCache::create_texture(fs::view("assets://textures/iron/rustediron-roughness.png")));*/

         //auto ent = createEntity(material.get_name());
         //ent.add_component<example_comp>();
         //auto [pos, rot, scal] = ent.add_component<transform>();
         //pos = math::vec3(0.f, 0.f, 0.f);
         //scal = math::vec3(3.f);
         //rot = math::angleAxis(math::half_pi<float>(), math::vec3::up);
         //ent.add_component(gfx::mesh_renderer(material, model));
    }
    //Rock
    {
        //material = gfx::MaterialCache::create_material("rock", fs::view("assets://shaders/pbr.shs"));

        //material.set_param(SV_ALBEDO, rendering::TextureCache::create_texture(fs::view("assets://textures/detailedRock/Rock020_1K_Albedo.png")));
        //material.set_param(SV_NORMALHEIGHT, rendering::TextureCache::create_texture(fs::view("assets://textures/detailedRock/Rock020_1K_NormalHeight.png")));
        //material.set_param(SV_MRDAO, rendering::TextureCache::create_texture(fs::view("assets://textures/detailedRock/Rock020_1K_MRDAo.png")));
        //material.set_param(SV_EMISSIVE, rendering::TextureCache::create_texture(fs::view("assets://textures/detailedRock/Rock020_1K_emissive.png")));
        //material.set_param(SV_HEIGHTSCALE, 1.f);

        //auto ent = createEntity(material.get_name());
        //ent.add_component<example_comp>();
        //auto [pos, rot, scal] = ent.add_component<transform>();
        //pos = math::vec3(0.f, 0.f, 4.f);
        //rot = math::angleAxis(math::half_pi<float>(), math::vec3::up);
        //scal = math::vec3(3.f);
        //ent.add_component(gfx::mesh_renderer(material, model));
    }
    //Copper
    {
        //material = gfx::MaterialCache::create_material("copper", fs::view("assets://shaders/pbr.shs"));

        //material.set_param(SV_ALBEDO, rendering::TextureCache::create_texture(fs::view("assets://textures/copper/copper-albedo-2048.png")));
        //material.set_param(SV_NORMALHEIGHT, rendering::TextureCache::create_texture(fs::view("assets://textures/copper/copper-normalHeight-2048.png")));
        //material.set_param(SV_MRDAO, rendering::TextureCache::create_texture(fs::view("assets://textures/copper/copper-MRDAo-2048.png")));
        //material.set_param(SV_EMISSIVE, rendering::TextureCache::create_texture(fs::view("assets://textures/copper/copper-emissive-2048.png")));
        //material.set_param(SV_HEIGHTSCALE, 1.f);

        //auto ent = createEntity(material.get_name());
        //ent.add_component<example_comp>();
        //auto [pos, rot, scal] = ent.add_component<transform>();
        //pos = math::vec3(0.f, 0.f, 8.f);
        //rot = math::angleAxis(math::half_pi<float>(), math::vec3::up);
        //scal = math::vec3(3.f);
        //ent.add_component(gfx::mesh_renderer(material, model));
    }
    //Wood
    {
        //material = gfx::MaterialCache::create_material("wood", fs::view("engine://shaders/default_lit.shs"));

        //material.set_param("alphaCutoff", 0.5f);
        //material.set_param("useAlbedoTex", true);
        //material.set_param("useRoughnessTex", true);
        //material.set_param("useNormal", true);
        //material.set_param("useHeight", true);

        //material.set_param("useAmbientOcclusion", false);
        //material.set_param("useMetallicTex", false);
        //material.set_param("useEmissiveTex", false);
        //material.set_param("useMetallicRoughness", false);

        //material.set_param("heightTex", rendering::TextureCache::create_texture(fs::view("assets://textures/wood/Wood035_1K_Displacement.png")));
        //material.set_param("heightScale", 1.f);
        //material.set_param("metallicValue", 0.f);
        //material.set_param("emissiveColor", math::colors::black);

        //material.set_param("albedoTex", rendering::TextureCache::create_texture(fs::view("assets://textures/wood/WoodColor.png")));
        //material.set_param("normalTex", rendering::TextureCache::create_texture(fs::view("assets://textures/wood/Wood035_1K_Normal.png")));
        //material.set_param("roughnessTex", rendering::TextureCache::create_texture(fs::view("assets://textures/wood/Wood035_1K_Roughness.png")));

        //auto ent = createEntity(material.get_name());
        //ent.add_component<example_comp>();
        //auto [pos, rot, scal] = ent.add_component<transform>();
        //pos = math::vec3(0.f, 4.f, 0.f);
        //rot = math::angleAxis(math::half_pi<float>(), math::vec3::up);
        //scal = math::vec3(3.f);
        //ent.add_component(gfx::mesh_renderer(material, model));
    }
    //Paint
    {
        //material = gfx::MaterialCache::create_material("paint", fs::view("assets://shaders/pbr.shs"));

        //material.set_param(SV_ALBEDO, rendering::TextureCache::create_texture(fs::view("assets://textures/paint/paint-peeling-albedo-2048.png")));
        //material.set_param(SV_NORMALHEIGHT, rendering::TextureCache::create_texture(fs::view("assets://textures/paint/paint-peeling-normalHeight-2048.png")));
        //material.set_param(SV_MRDAO, rendering::TextureCache::create_texture(fs::view("assets://textures/paint/paint-peeling-MRDAo-2048.png")));
        //material.set_param(SV_EMISSIVE, rendering::TextureCache::create_texture(fs::view("assets://textures/paint/paint-peeling-emissive-2048.png")));
        //material.set_param(SV_HEIGHTSCALE, 1.f);

        //auto ent = createEntity(material.get_name());
        //ent.add_component<example_comp>();
        //auto [pos, rot, scal] = ent.add_component<transform>();
        //pos = math::vec3(0.f, 4.f, 4.f);
        //rot = math::angleAxis(math::half_pi<float>(), math::vec3::up);
        //scal = math::vec3(3.f);
        //ent.add_component(gfx::mesh_renderer(material, model));
    }
    //Tiles
    {
        //material = gfx::MaterialCache::create_material("tiles", fs::view("engine://shaders/default_lit.shs"));

        //material.set_param("alphaCutoff", 0.5f);
        //material.set_param("useAlbedoTex", true);
        //material.set_param("useRoughnessTex", true);
        //material.set_param("useNormal", true);

        //material.set_param("useMetallicTex", false);
        //material.set_param("useHeight", false);
        //material.set_param("useAmbientOcclusion", false);
        //material.set_param("useEmissiveTex", false);
        //material.set_param("useMetallicRoughness", false);

        //material.set_param("metallicValue", 0.f);
        //material.set_param("emissiveColor", math::colors::black);

        //material.set_param("albedoTex", rendering::TextureCache::create_texture(fs::view("assets://textures/tile/tileColor.png")));
        //material.set_param("normalTex", rendering::TextureCache::create_texture(fs::view("assets://textures/tile/tileNormal.png")));
        //material.set_param("roughnessTex", rendering::TextureCache::create_texture(fs::view("assets://textures/tile/tileRoughness.png")));

        //auto ent = createEntity(material.get_name());
        //ent.add_component<example_comp>();
        //auto [pos, rot, scal] = ent.add_component<transform>();
        //pos = math::vec3(0.f, 4.f, 8.f);
        //rot = math::angleAxis(math::half_pi<float>(), math::vec3::up);
        //scal = math::vec3(3.f);
        //ent.add_component(gfx::mesh_renderer(material, model));
    }
    //Slate
    {
        //material = gfx::MaterialCache::create_material("slate", fs::view("assets://shaders/pbr.shs"));

        //material.set_param(SV_ALBEDO, rendering::TextureCache::create_texture(fs::view("assets://textures/slate/slate-albedo-2048.png")));
        //material.set_param(SV_NORMALHEIGHT, rendering::TextureCache::create_texture(fs::view("assets://textures/slate/slate-normalHeight-2048.png")));
        //material.set_param(SV_MRDAO, rendering::TextureCache::create_texture(fs::view("assets://textures/slate/slate-MRDAo-2048.png")));
        //material.set_param(SV_EMISSIVE, rendering::TextureCache::create_texture(fs::view("assets://textures/slate/slate-emissive-2048.png")));
        //material.set_param(SV_HEIGHTSCALE, 1.f);

        //auto ent = createEntity(material.get_name());
        //ent.add_component<example_comp>();
        //auto [pos, rot, scal] = ent.add_component<transform>();
        //pos = math::vec3(0.f, 8.f, 0.f);
        //rot = math::angleAxis(math::half_pi<float>(), math::vec3::up);
        //scal = math::vec3(3.f);
        //ent.add_component(gfx::mesh_renderer(material, model));
    }
    //Bog
    {
        material = gfx::MaterialCache::create_material("bog", fs::view("assets://shaders/pbr.shs"));

        material.set_param(SV_ALBEDO, rendering::TextureCache::create_texture(fs::view("assets://textures/bog/bog-albedo-2048.png")));
        material.set_param(SV_NORMALHEIGHT, rendering::TextureCache::create_texture(fs::view("assets://textures/bog/bog-normalHeight-2048.png")));
        material.set_param(SV_MRDAO, rendering::TextureCache::create_texture(fs::view("assets://textures/bog/bog-MRDAo-2048.png")));
        material.set_param(SV_EMISSIVE, rendering::TextureCache::create_texture(fs::view("assets://textures/bog/bog-emissive-2048.png")));
        material.set_param(SV_HEIGHTSCALE, 1.f);
    }
    //Bricks
    {
        //material = gfx::MaterialCache::create_material("bricks", fs::view("engine://shaders/default_lit.shs"));

        //material.set_param("alphaCutoff", 0.5f);
        //material.set_param("useAlbedoTex", true);
        //material.set_param("useRoughnessTex", true);
        //material.set_param("useNormal", true);
        //material.set_param("useHeight", true);
        //material.set_param("useAmbientOcclusion", true);

        //material.set_param("useMetallicTex", false);
        //material.set_param("useEmissiveTex", false);
        //material.set_param("useMetallicRoughness", false);

        //material.set_param("metallicValue", 0.f);
        //material.set_param("emissiveColor", math::colors::black);

        //material.set_param("albedoTex", rendering::TextureCache::create_texture(fs::view("assets://textures/brick/brickColor.png")));
        //material.set_param("roughnessTex", rendering::TextureCache::create_texture(fs::view("assets://textures/brick/brickRoughness.png")));
        //material.set_param("normalTex", rendering::TextureCache::create_texture(fs::view("assets://textures/brick/brickNormal.png")));
        //material.set_param("ambientOcclusionTex", rendering::TextureCache::create_texture(fs::view("assets://textures/brick/brickAO.png")));
        //material.set_param("heightTex", rendering::TextureCache::create_texture(fs::view("assets://textures/brick/brickDisplacement.png")));
        //material.set_param("heightScale", 0.1f);

        //auto ent = createEntity(material.get_name());
        //ent.add_component<example_comp>();
        //auto [pos, rot, scal] = ent.add_component<transform>();
        //pos = math::vec3(0.f, 8.f, 4.f);
        //rot = math::angleAxis(math::half_pi<float>(), math::vec3::up);
        //scal = math::vec3(3.f);
        //ent.add_component(gfx::mesh_renderer(material, model));
    }
    //Default
    {
        //material = gfx::MaterialCache::create_material("default", fs::view("assets://shaders/pbr.shs"));

        //material.set_param(SV_ALBEDO, rendering::TextureCache::create_texture(fs::view("engine://resources/default/albedo")));
        //material.set_param(SV_NORMALHEIGHT, rendering::TextureCache::create_texture(fs::view("engine://resources/default/normalHeight")));
        //material.set_param(SV_MRDAO, rendering::TextureCache::create_texture(fs::view("engine://resources/default/MRDAo")));
        //material.set_param(SV_EMISSIVE, rendering::TextureCache::create_texture(fs::view("engine://resources/default/emissive")));
        //material.set_param(SV_HEIGHTSCALE, 1.f);

        //auto ent = createEntity(material.get_name());
        //ent.add_component<example_comp>();
        //auto [pos, rot, scal] = ent.add_component<transform>();
        //pos = math::vec3(0.f, 8.f, 8.f);
        //rot = math::angleAxis(math::half_pi<float>(), math::vec3::up);
        //scal = math::vec3(3.f);
        //ent.add_component(gfx::mesh_renderer(material, model));
    }
    //Sun
    {
        //auto ent = createEntity("Sun");
        //auto [pos, rot, scal] = ent.add_component<transform>();
        //pos->x = -30.f;
        //pos->z = 30.f;
        //scal = scale(10.f);
        //material = gfx::MaterialCache::create_material("Sun", fs::view("assets://shaders/pbr.shs"));
        ////gfx::ModelCache::create_model("Sponza", fs::view("assets://models/Sponza/Sponza.gltf"));
        //gfx::ModelCache::create_model("Cola", fs::view("assets://models/cola.glb"));
        ////gfx::ModelCache::create_model("Fireplace", fs::view("assets://models/Fireplace.glb"));
        //gfx::ModelCache::create_model("Gnome", fs::view("assets://models/gnomecentered.obj"));
        //gfx::MaterialCache::create_material("Test", fs::view("assets://shaders/uv.shs"));

        //material.set_param(SV_ALBEDO, rendering::TextureCache::create_texture(fs::view("assets://textures/2kSun.jpg")));
        //material.set_param(SV_NORMALHEIGHT, rendering::TextureCache::create_texture(fs::view("assets://textures/slate/slate-normalHeight-2048.png")));
        //material.set_param(SV_MRDAO, rendering::TextureCache::create_texture(fs::view("assets://textures/slate/slate-MRDAo-2048.png")));
        //material.set_param(SV_EMISSIVE, rendering::TextureCache::create_texture(fs::view("assets://textures/2kSun.jpg")));
        //material.set_param(SV_HEIGHTSCALE, 1.f);

        //ent.add_component<gfx::mesh_renderer>(gfx::mesh_renderer(material, model));
        //ent.add_component(gfx::light::point(math::color(1.f, 215.f / 255.f, 0.f), 10.f, 50.f));
    }

    ////Orbits
    //{
    //    auto ent = createEntity("Saturn");
    //    auto [pos, rot, scal] = ent.add_component<transform>();
    //    scal = scale(5.f, 5.f, 5.f);
    //    material = gfx::MaterialCache::get_material("bog");
    //    model = gfx::ModelCache::get_handle("Sphere");
    //    ent.add_component<gfx::mesh_renderer>(gfx::mesh_renderer(material, model));
    //    auto orbit = createEntity("orbitRings");
    //    ent.add_child(orbit);

    //    auto emitter = orbit.add_component<particle_emitter>();
    //    emitter->set_spawn_rate(100);
    //    emitter->set_spawn_interval(0.2f);
    //    emitter->resize(10000);
    //    emitter->localSpace = true;

    //    emitter->add_policy<example_policy>();
    //    orbital_policy orbital;
    //    orbital.C_MASS = 100.f;
    //    orbital.G_FORCE = .1f;
    //    emitter->add_policy<orbital_policy>(orbital);
    //    emitter->add_policy<scale_lifetime_policy>();
    //    material = gfx::MaterialCache::get_material("slate");
    //    emitter->add_policy<gfx::rendering_policy>(gfx::rendering_policy{ model, material });
    //}

    auto ent = createEntity();
    ent.add_component<gfx::mesh_renderer>(gfx::mesh_renderer(material, model));
    ent.add_component<transform>();

    auto emitter = ent.add_component<particle_emitter>();
    emitter->set_spawn_rate(1000);
    emitter->set_spawn_interval(.02f);
    //emitter->create_uniform<float>("minLifeTime") = 1.f;
    //emitter->create_uniform<float>("maxLifeTime") = 5.f;
    //emitter->create_uniform<int>("frameCount", 9);
    emitter->resize(100000);
    ////emitter->add_policy<scale_lifetime_policy>();
    emitter->add_policy<example_policy>();
    //emitter->add_policy<gpu_orbital_policy>();
    //emitter->add_policy<fountain_policy>();
    //emitter->add_policy<gpu_fountain_policy>();
    material = gfx::MaterialCache::create_material("Particle", fs::view("assets://shaders/particle.shs"));
    material.set_param("isLit", false);
    material.set_param("billboard", false);
    material.set_param("animated", false);
    material.set_param("useSolidColor", true);
    material.set_param("_color", math::color(0.f, 0.f, 1.f, .5f));
    material.set_param("useTexture", false);

    material.set_param(SV_ALBEDO, rendering::TextureCache::create_texture(fs::view("engine://resources/default/albedo")));
    material.set_param(SV_NORMALHEIGHT, rendering::TextureCache::create_texture(fs::view("engine://resources/default/normalHeight")));
    material.set_param(SV_MRDAO, rendering::TextureCache::create_texture(fs::view("engine://resources/default/MRDAo")));
    material.set_param(SV_EMISSIVE, rendering::TextureCache::create_texture(fs::view("engine://resources/default/emissive")));
    material.set_param(SV_HEIGHTSCALE, 1.f);

    //auto textureArray = gfx::TextureCache::create_texture_array("Explosion",
    //{
    //    fs::view("assets://textures/explosion/frame0.png"),
    //    fs::view("assets://textures/explosion/frame1.png"),
    //    fs::view("assets://textures/explosion/frame2.png"),
    //    fs::view("assets://textures/explosion/frame3.png"),
    //    fs::view("assets://textures/explosion/frame4.png"),
    //    fs::view("assets://textures/explosion/frame5.png"),
    //    fs::view("assets://textures/explosion/frame6.png"),
    //    fs::view("assets://textures/explosion/frame7.png"),
    //    fs::view("assets://textures/explosion/frame8.png"),
    //});

    //material.set_param("_texture", textureArray);
    //material.set_param("frameCount", emitter->get_uniform<int>("frameCount"));
    //model = gfx::ModelCache::create_model("Billboard", fs::view("assets://models/sphere.obj"));
    emitter->add_policy<gfx::rendering_policy>(gfx::rendering_policy{ model, material });
    //emitter->add_policy<gfx::flipbook_policy>();

    //    emitter->add_policy<example_policy>();
    //    orbital_policy orbital;
    //    orbital.C_MASS = 100.f;
    //    orbital.G_FORCE = .1f;
    //    emitter->add_policy<orbital_policy>(orbital);
    //    material = gfx::MaterialCache::create_material("AnimParticle", fs::view("assets://shaders/particle.shs"));
    //    material.set_param("_texture", gfx::TextureCache::create_texture_array("Explosion",
    //        {
    //            fs::view("assets://textures/explosion/frame0.png"),
    //            fs::view("assets://textures/explosion/frame1.png"),
    //            fs::view("assets://textures/explosion/frame2.png"),
    //            fs::view("assets://textures/explosion/frame3.png"),
    //            fs::view("assets://textures/explosion/frame4.png"),
    //            fs::view("assets://textures/explosion/frame5.png"),
    //            fs::view("assets://textures/explosion/frame6.png"),
    //            fs::view("assets://textures/explosion/frame7.png"),
    //            fs::view("assets://textures/explosion/frame8.png"),
    //        }));
    //    model = gfx::ModelCache::create_model("Billboard", fs::view("assets://models/billboard.glb"));
    //    emitter->add_policy<gfx::rendering_policy>(gfx::rendering_policy{ model, material });
    //    emitter->add_policy<gfx::flipbook_policy>();
    //}

        //Boids
    {
        //auto ent = createEntity("Boids");
        //auto [pos, rot, scal] = ent.add_component<transform>();
        //material = gfx::MaterialCache::get_material("White");
        //model = gfx::ModelCache::get_handle("ParticleGizmo");
        //ent.add_component<gfx::mesh_renderer>(gfx::mesh_renderer(material, model));

        //auto emitter = ent.add_component<particle_emitter>();
        //emitter->set_spawn_rate(10);
        //emitter->set_spawn_interval(.05f);
        //emitter->create_uniform<float>("minLifeTime") = 9.f;
        //emitter->create_uniform<float>("maxLifeTime") = 9.f;
        //emitter->create_uniform<uint>("frameCount", 9);
        //emitter->resize(1000);
        //emitter->localSpace = false;
        //emitter->pause();
        //emitter->add_policy<locomotion_policy>();
        //emitter->add_policy<alignment_policy>();
        //emitter->add_policy<cohesion_policy>();
        //emitter->add_policy<seperation_policy>();
        //material = gfx::MaterialCache::create_material("AnimParticle", fs::view("assets://shaders/particle.shs"));
        //auto textureArray = gfx::TextureCache::create_texture_array("Explosion",
        //    {
        //        fs::view("assets://textures/explosion/frame0.png"),
        //        fs::view("assets://textures/explosion/frame1.png"),
        //        fs::view("assets://textures/explosion/frame2.png"),
        //        fs::view("assets://textures/explosion/frame3.png"),
        //        fs::view("assets://textures/explosion/frame4.png"),
        //        fs::view("assets://textures/explosion/frame5.png"),
        //        fs::view("assets://textures/explosion/frame6.png"),
        //        fs::view("assets://textures/explosion/frame7.png"),
        //        fs::view("assets://textures/explosion/frame8.png"),
        //    });
        //material.set_param("_texture", textureArray);
        //material.set_variant("depth_only");
        //material.set_param("_texture", textureArray);
        //material.set_variant("default");
        //model = gfx::ModelCache::create_model("Billboard", fs::view("assets://models/billboard.glb"));
        //emitter->add_policy<gfx::rendering_policy>(gfx::rendering_policy{ model, material });
        //emitter->add_policy<gfx::flipbook_policy>();
    }

    {

        //auto parent = createEntity("Parent");
        //parent.add_component<transform>();
        //material = gfx::MaterialCache::create_material("parentTex", fs::view("assets://shaders/texture.shs"));
        //material.set_param("_texture", gfx::TextureCache::create_texture(fs::view("assets://textures/explosion/frame0.png")));
        //model = gfx::ModelCache::get_handle("ParticleGizmo");
        //parent.add_component<gfx::mesh_renderer>(gfx::mesh_renderer(material, model));

        //auto ent = createEntity("Fountain");
        //auto [pos, rot, scal] = ent.add_component<transform>();
        //auto emitter = ent.add_component<particle_emitter>();
        //emitter->set_spawn_rate(9);
        //emitter->set_spawn_interval(.05f);
        //emitter->resize(900);
        //emitter->create_uniform<float>("minLifeTime") = 1.0f;
        //emitter->create_uniform<float>("maxLifeTime") = 5.0f;
        //emitter->create_uniform<int>("frameCount", 9);
        //emitter->set_particle_space(WORLD);

        //fountain_policy fountain;
        //fountain.initForce = 0.5f;
        //emitter->add_policy<fountain_policy>(fountain);
        //emitter->add_policy<scale_lifetime_policy>();
        //material = gfx::MaterialCache::create_material("AnimParticle", fs::view("assets://shaders/particle.shs"));
        //auto textureArray = gfx::TextureCache::create_texture_array("Explosion",
        //    {
        //        fs::view("assets://textures/explosion/frame0.png"),
        //        fs::view("assets://textures/explosion/frame1.png"),
        //        fs::view("assets://textures/explosion/frame2.png"),
        //        fs::view("assets://textures/explosion/frame3.png"),
        //        fs::view("assets://textures/explosion/frame4.png"),
        //        fs::view("assets://textures/explosion/frame5.png"),
        //        fs::view("assets://textures/explosion/frame6.png"),
        //        fs::view("assets://textures/explosion/frame7.png"),
        //        fs::view("assets://textures/explosion/frame8.png"),
        //    });
        //material.set_param("_texture", textureArray);
        //material.set_param("frameCount", emitter->get_uniform<int>("frameCount"));
        //material.set_variant("depth_only");
        //material.set_param("_texture", textureArray);
        //material.set_variant("default");
        //model = gfx::ModelCache::create_model("Billboard", fs::view("assets://models/billboard.glb"));
        //emitter->add_policy<gfx::rendering_policy>(gfx::rendering_policy{ model, material });
        //emitter->add_policy<gfx::flipbook_policy>();
    }

    app::InputSystem::createBinding<tonemap_action>(app::inputmap::method::F2);
    app::InputSystem::createBinding<reload_shaders_action>(app::inputmap::method::F3);
    app::InputSystem::createBinding<switch_skybox_action>(app::inputmap::method::F4);
    app::InputSystem::createBinding<play_action>(app::inputmap::method::F5);
    app::InputSystem::createBinding<pause_action>(app::inputmap::method::F6);
    app::InputSystem::createBinding<stop_action>(app::inputmap::method::F7);
    app::InputSystem::createBinding<change_mat_action>(app::inputmap::method::F8);

    bindToEvent<tonemap_action, &ExampleSystem::onTonemapSwitch>();
    bindToEvent<reload_shaders_action, &ExampleSystem::onShaderReload>();
    bindToEvent<switch_skybox_action, &ExampleSystem::onSkyboxSwitch>();
    bindToEvent<events::exit, &ExampleSystem::onExit>();
    bindToEvent<play_action, &ExampleSystem::playEmitter>();
    bindToEvent<pause_action, &ExampleSystem::pauseEmitter>();
    bindToEvent<stop_action, &ExampleSystem::stopEmitter>();
    bindToEvent<change_mat_action, &ExampleSystem::changeMaterial>();


    bindToEvent<events::exit, &ExampleSystem::onExit>();
}

void ExampleSystem::playEmitter(legion::play_action& action)
{
    using namespace legion;
    if (action.pressed())
    {
        if (GuiTestSystem::selected != invalid_id)
        {
            if (GuiTestSystem::selected.has_component<particle_emitter>())
            {
                auto& emitter = GuiTestSystem::selected.get_component<particle_emitter>().get();
                emitter.play();
            }
        }
    }
}

void ExampleSystem::pauseEmitter(legion::pause_action& action)
{
    using namespace legion;
    if (action.pressed())
    {
        if (GuiTestSystem::selected != invalid_id)
        {
            if (GuiTestSystem::selected.has_component<particle_emitter>())
            {
                auto& emitter = GuiTestSystem::selected.get_component<particle_emitter>().get();
                emitter.pause();
            }
        }
    }
}

void ExampleSystem::stopEmitter(legion::stop_action& action)
{
    using namespace legion;
    if (action.pressed())
    {
        if (GuiTestSystem::selected != invalid_id)
        {
            if (GuiTestSystem::selected.has_component<particle_emitter>())
            {
                auto& emitter = GuiTestSystem::selected.get_component<particle_emitter>().get();
                emitter.stop();
            }
        }
    }
}

void ExampleSystem::changeMaterial(legion::change_mat_action& action)
{
    using namespace legion;
    if (action.pressed())
    {
        if (GuiTestSystem::selected != invalid_id)
        {
            auto& emitter = GuiTestSystem::selected.get_component<particle_emitter>().get();
            auto models = gfx::ModelCache::get_all_models();
            auto randModel = gfx::ModelCache::get_handle(models.keys()[std::rand() % models.size()]);
            auto randMesh = randModel.get_mesh();
            log::debug("Random Model Name: {}", randModel.get_mesh().name());
            if (emitter.has_uniform<mesh_filter>("mesh_filter"))
                emitter.get_uniform<mesh_filter>("mesh_filter") = mesh_filter(randMesh);

            auto& mats = gfx::MaterialCache::get_all_materials().second;
            auto pair = std::next(mats.begin(), std::rand() % mats.size());
            auto randMat = gfx::MaterialCache::get_material(pair->first);
            log::debug("Random Material Name: {}", pair->second.get_name());
            if (emitter.has_uniform<gfx::mesh_renderer>("renderer"))
                emitter.get_uniform<gfx::mesh_renderer>("renderer") = gfx::mesh_renderer(randMat, randModel);
        }
    }
}

void ExampleSystem::onShaderReload(legion::reload_shaders_action& event)
{
    using namespace legion;
    if (event.pressed())
    {
        auto targetWin = ecs::world.get_component<app::window>();
        if (!targetWin)
            return;

        app::window& win = targetWin.get();

        if (!app::WindowSystem::windowStillExists(win.handle))
            return;

        app::context_guard guard(win);
        if (guard.contextIsValid())
        {
            gfx::ShaderCache::clear_checked_paths();
            gfx::ShaderCache::reload_shaders();
        }
    }
}

void ExampleSystem::onTonemapSwitch(legion::tonemap_action& event)
{
    using namespace legion;
    if (event.pressed())
    {
        static size_type type = static_cast<size_type>(gfx::tonemapping_type::legion);
        type = (type + 1) % (static_cast<size_type>(gfx::tonemapping_type::unreal3) + 1);

        auto typeEnum = static_cast<gfx::tonemapping_type>(type);

        std::string algorithmName;
        switch (typeEnum)
        {
        case gfx::tonemapping_type::aces:
            algorithmName = "aces";
            break;
        case gfx::tonemapping_type::reinhard:
            algorithmName = "reinhard";
            break;
        case gfx::tonemapping_type::reinhard_jodie:
            algorithmName = "reinhard_jodie";
            break;
        case gfx::tonemapping_type::legion:
            algorithmName = "legion";
            break;
        case gfx::tonemapping_type::unreal3:
            algorithmName = "unreal3";
            break;
        default:
            algorithmName = "legion";
            break;
        }
        log::debug("Set tonemapping algorithm to {}", algorithmName);

        gfx::Tonemapping::setAlgorithm(typeEnum);
    }
}

void ExampleSystem::onSkyboxSwitch(legion::switch_skybox_action& event)
{
    using namespace legion;
    if (event.pressed())
    {
        static size_type idx = 0;
        static gfx::texture_handle textures[4] = {};
        static bool initialized = false;

        if (!initialized)
        {
            auto targetWin = ecs::world.get_component<app::window>();
            if (!targetWin)
                return;

            app::window& win = targetWin.get();

            if (!app::WindowSystem::windowStillExists(win.handle))
                return;

            app::context_guard guard(win);
            if (guard.contextIsValid())
            {
                textures[0] = gfx::TextureCache::create_texture("morning islands", fs::view("assets://textures/HDRI/morning_islands.jpg"));
                textures[1] = gfx::TextureCache::create_texture("earth", fs::view("assets://textures/HDRI/earth.png"));
                textures[2] = gfx::TextureCache::create_texture("park", fs::view("assets://textures/HDRI/park.jpg"));
                textures[3] = gfx::TextureCache::create_texture("atmosphere", fs::view("assets://textures/HDRI/planetatmo.png"));
                initialized = true;
            }
            else
                return;
        }

        idx = (idx + 1) % 4;
        auto skyboxRenderer = ecs::world.get_component<gfx::skybox_renderer>();
        skyboxRenderer->material.set_param(SV_SKYBOX, textures[idx]);

        log::debug("Set skybox to {}", textures[idx].get_texture().name);
    }
}
