#include <chrono>

#include "../systems/examplesystem.hpp"
#include "../defaults/defaultpolicies.hpp"


void ExampleSystem::setup()
{
    using namespace legion;
    log::filter(log::severity_debug);
    log::debug("ExampleSystem setup");

    app::window& win = ecs::world.get_component<app::window>();
    app::context_guard guard(win);

    auto model = gfx::ModelCache::create_model("Sphere", fs::view("assets://models/sphere.obj"));

    auto material = gfx::MaterialCache::create_material("White", fs::view("assets://shaders/color.shs"));
    material.set_param("color", math::colors::white);

    //Iron
    {
        material = gfx::MaterialCache::create_material("iron", fs::view("engine://shaders/default_lit.shs"));

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
        material.set_param("roughnessTex", rendering::TextureCache::create_texture(fs::view("assets://textures/iron/rustediron-roughness.png")));

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
        material = gfx::MaterialCache::create_material("rock", fs::view("assets://shaders/pbr.shs"));

        material.set_param(SV_ALBEDO, rendering::TextureCache::create_texture(fs::view("assets://textures/detailedRock/Rock020_1K_Albedo.png")));
        material.set_param(SV_NORMALHEIGHT, rendering::TextureCache::create_texture(fs::view("assets://textures/detailedRock/Rock020_1K_NormalHeight.png")));
        material.set_param(SV_MRDAO, rendering::TextureCache::create_texture(fs::view("assets://textures/detailedRock/Rock020_1K_MRDAo.png")));
        material.set_param(SV_EMISSIVE, rendering::TextureCache::create_texture(fs::view("assets://textures/detailedRock/Rock020_1K_emissive.png")));
        material.set_param(SV_HEIGHTSCALE, 1.f);

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
        material = gfx::MaterialCache::create_material("copper", fs::view("assets://shaders/pbr.shs"));

        material.set_param(SV_ALBEDO, rendering::TextureCache::create_texture(fs::view("assets://textures/copper/copper-albedo-2048.png")));
        material.set_param(SV_NORMALHEIGHT, rendering::TextureCache::create_texture(fs::view("assets://textures/copper/copper-normalHeight-2048.png")));
        material.set_param(SV_MRDAO, rendering::TextureCache::create_texture(fs::view("assets://textures/copper/copper-MRDAo-2048.png")));
        material.set_param(SV_EMISSIVE, rendering::TextureCache::create_texture(fs::view("assets://textures/copper/copper-emissive-2048.png")));
        material.set_param(SV_HEIGHTSCALE, 1.f);

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
        material = gfx::MaterialCache::create_material("wood", fs::view("engine://shaders/default_lit.shs"));

        material.set_param("alphaCutoff", 0.5f);
        material.set_param("useAlbedoTex", true);
        material.set_param("useRoughnessTex", true);
        material.set_param("useNormal", true);
        material.set_param("useHeight", true);

        material.set_param("useAmbientOcclusion", false);
        material.set_param("useMetallicTex", false);
        material.set_param("useEmissiveTex", false);
        material.set_param("useMetallicRoughness", false);

        material.set_param("heightTex", rendering::TextureCache::create_texture(fs::view("assets://textures/wood/Wood035_1K_Displacement.png")));
        material.set_param("heightScale", 1.f);
        material.set_param("metallicValue", 0.f);
        material.set_param("emissiveColor", math::colors::black);

        material.set_param("albedoTex", rendering::TextureCache::create_texture(fs::view("assets://textures/wood/WoodColor.png")));
        material.set_param("normalTex", rendering::TextureCache::create_texture(fs::view("assets://textures/wood/Wood035_1K_Normal.png")));
        material.set_param("roughnessTex", rendering::TextureCache::create_texture(fs::view("assets://textures/wood/Wood035_1K_Roughness.png")));

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
        material = gfx::MaterialCache::create_material("paint", fs::view("assets://shaders/pbr.shs"));

        material.set_param(SV_ALBEDO, rendering::TextureCache::create_texture(fs::view("assets://textures/paint/paint-peeling-albedo-2048.png")));
        material.set_param(SV_NORMALHEIGHT, rendering::TextureCache::create_texture(fs::view("assets://textures/paint/paint-peeling-normalHeight-2048.png")));
        material.set_param(SV_MRDAO, rendering::TextureCache::create_texture(fs::view("assets://textures/paint/paint-peeling-MRDAo-2048.png")));
        material.set_param(SV_EMISSIVE, rendering::TextureCache::create_texture(fs::view("assets://textures/paint/paint-peeling-emissive-2048.png")));
        material.set_param(SV_HEIGHTSCALE, 1.f);

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
        material = gfx::MaterialCache::create_material("tiles", fs::view("engine://shaders/default_lit.shs"));

        material.set_param("alphaCutoff", 0.5f);
        material.set_param("useAlbedoTex", true);
        material.set_param("useRoughnessTex", true);
        material.set_param("useNormal", true);

        material.set_param("useMetallicTex", false);
        material.set_param("useHeight", false);
        material.set_param("useAmbientOcclusion", false);
        material.set_param("useEmissiveTex", false);
        material.set_param("useMetallicRoughness", false);

        material.set_param("metallicValue", 0.f);
        material.set_param("emissiveColor", math::colors::black);

        material.set_param("albedoTex", rendering::TextureCache::create_texture(fs::view("assets://textures/tile/tileColor.png")));
        material.set_param("normalTex", rendering::TextureCache::create_texture(fs::view("assets://textures/tile/tileNormal.png")));
        material.set_param("roughnessTex", rendering::TextureCache::create_texture(fs::view("assets://textures/tile/tileRoughness.png")));

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
        material = gfx::MaterialCache::create_material("slate", fs::view("assets://shaders/pbr.shs"));

        material.set_param(SV_ALBEDO, rendering::TextureCache::create_texture(fs::view("assets://textures/slate/slate-albedo-2048.png")));
        material.set_param(SV_NORMALHEIGHT, rendering::TextureCache::create_texture(fs::view("assets://textures/slate/slate-normalHeight-2048.png")));
        material.set_param(SV_MRDAO, rendering::TextureCache::create_texture(fs::view("assets://textures/slate/slate-MRDAo-2048.png")));
        material.set_param(SV_EMISSIVE, rendering::TextureCache::create_texture(fs::view("assets://textures/slate/slate-emissive-2048.png")));
        material.set_param(SV_HEIGHTSCALE, 1.f);

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
        material = gfx::MaterialCache::create_material("bricks", fs::view("engine://shaders/default_lit.shs"));

        material.set_param("alphaCutoff", 0.5f);
        material.set_param("useAlbedoTex", true);
        material.set_param("useRoughnessTex", true);
        material.set_param("useNormal", true);
        material.set_param("useHeight", true);
        material.set_param("useAmbientOcclusion", true);

        material.set_param("useMetallicTex", false);
        material.set_param("useEmissiveTex", false);
        material.set_param("useMetallicRoughness", false);

        material.set_param("metallicValue", 0.f);
        material.set_param("emissiveColor", math::colors::black);

        material.set_param("albedoTex", rendering::TextureCache::create_texture(fs::view("assets://textures/brick/brickColor.png")));
        material.set_param("roughnessTex", rendering::TextureCache::create_texture(fs::view("assets://textures/brick/brickRoughness.png")));
        material.set_param("normalTex", rendering::TextureCache::create_texture(fs::view("assets://textures/brick/brickNormal.png")));
        material.set_param("ambientOcclusionTex", rendering::TextureCache::create_texture(fs::view("assets://textures/brick/brickAO.png")));
        material.set_param("heightTex", rendering::TextureCache::create_texture(fs::view("assets://textures/brick/brickDisplacement.png")));
        material.set_param("heightScale", 0.1f);

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
        material = gfx::MaterialCache::create_material("default", fs::view("assets://shaders/pbr.shs"));

        material.set_param(SV_ALBEDO, rendering::TextureCache::create_texture(fs::view("engine://resources/default/albedo")));
        material.set_param(SV_NORMALHEIGHT, rendering::TextureCache::create_texture(fs::view("engine://resources/default/normalHeight")));
        material.set_param(SV_MRDAO, rendering::TextureCache::create_texture(fs::view("engine://resources/default/MRDAo")));
        material.set_param(SV_EMISSIVE, rendering::TextureCache::create_texture(fs::view("engine://resources/default/emissive")));
        material.set_param(SV_HEIGHTSCALE, 1.f);

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
       /* auto ent = createEntity("Sun");
        auto [pos, rot, scal] = ent.add_component<transform>();
        pos->x = -30.f;
        pos->z = 30.f;
        scal = scale(10.f);
        material = gfx::MaterialCache::create_material("Sun", fs::view("assets://shaders/pbr.shs"));

        material.set_param(SV_ALBEDO, rendering::TextureCache::create_texture(fs::view("assets://textures/2kSun.jpg")));
        material.set_param(SV_NORMALHEIGHT, rendering::TextureCache::create_texture(fs::view("assets://textures/slate/slate-normalHeight-2048.png")));
        material.set_param(SV_MRDAO, rendering::TextureCache::create_texture(fs::view("assets://textures/slate/slate-MRDAo-2048.png")));
        material.set_param(SV_EMISSIVE, rendering::TextureCache::create_texture(fs::view("assets://textures/2kSun.jpg")));
        material.set_param(SV_HEIGHTSCALE, 1.f);

        ent.add_component<gfx::mesh_renderer>(gfx::mesh_renderer(material, model));
        ent.add_component(gfx::light::point(math::color(1.f, 215.f / 255.f, 0.f), 10.f, 50.f));*/
    }

    {
        auto ent = createEntity("Saturn");
        auto [pos, rot, scal] = ent.add_component<transform>();
        scal = scale(5.f, 5.f, 5.f);
        material = gfx::MaterialCache::get_material("bog");
        model = gfx::ModelCache::get_handle("Sphere");
        ent.add_component<gfx::mesh_renderer>(gfx::mesh_renderer(material, model));
        auto orbit = createEntity("orbitRings");
        ent.add_child(orbit);

        auto emitter = orbit.add_component<particle_emitter>();
        emitter->set_spawn_rate(100);
        emitter->set_spawn_interval(0.2f);
        emitter->resize(10000);
        emitter->localSpace = false;

        emitter->add_policy<example_policy>();
        orbital_policy orbital;
        orbital.C_MASS = 100.f;
        orbital.G_FORCE = .1f;
        emitter->add_policy<orbital_policy>(orbital);
        emitter->add_policy<scale_lifetime_policy>();
        material = gfx::MaterialCache::get_material("slate");
        emitter->add_policy<gfx::rendering_policy>(gfx::rendering_policy{ model, material });
    }

    //{
    //    auto ent = createEntity("Fountain");
    //    auto [pos, rot, scal] = ent.add_component<transform>();
    //    pos = position(0, 7, 0);
    //    auto emitter = ent.add_component<particle_emitter>();
    //    emitter->set_spawn_rate(10);
    //    emitter->set_spawn_interval(0.2f);
    //    emitter->resize(50000);
    //    emitter->create_uniform<float>("minLifeTime") = 1.f;
    //    emitter->create_uniform<float>("maxLifeTime") = 2.f;
    //    emitter->localSpace = false;

    //    fountain_policy fountain;
    //    fountain.initForce = 20.f;
    //    emitter->add_policy<fountain_policy>(fountain);
    //    emitter->add_policy<scale_lifetime_policy>();
    //    emitter->add_policy<gfx::rendering_policy>(gfx::rendering_policy{ gfx::ModelCache::create_model("Quad", fs::view("assets://models/billboard.glb")),  gfx::MaterialCache::create_material("Particle", fs::view("assets://shaders/particle.shs")) });
    //    gfx::MaterialCache::get_material("Particle").set_param("particleColor", math::color(1.f, 1.f, 1.f, 1.f));
    //    gfx::MaterialCache::get_material("Particle").set_param("fixedSize", false);
    //}

    bindToEvent<events::exit, &ExampleSystem::onExit>();
}

