#include "../systems/examplesystem.hpp"
#include "../defaults/defaultpolicies.hpp"
#include <chrono>

void ExampleSystem::setup()
{
    using namespace legion;
    log::filter(log::severity_debug);
    log::debug("ExampleSystem setup");

    app::window& win = ecs::world.get_component<app::window>();
    app::context_guard guard(win);

    {
        auto ent = createEntity("Sun");
        ent.add_component(gfx::light::directional(math::color(1, 1, 0.8f), 10.f));
        auto [pos, rot, scal] = ent.add_component<transform>();
        rot = rotation::lookat(math::vec3::zero, math::vec3(-1, -1, -1));
    }

    {
        auto model = gfx::ModelCache::create_model("Sphere", fs::view("assets://models/sphere.obj"));
        auto material = gfx::MaterialCache::create_material("Pbr", fs::view("assets://shaders/pbr.shs"));
        material.set_param(SV_ALBEDO, gfx::TextureCache::create_texture(fs::view("engine://resources/default/albedo")));
        material.set_param(SV_NORMALHEIGHT, gfx::TextureCache::create_texture(fs::view("engine://resources/default/normalHeight")));
        material.set_param(SV_MRDAO, gfx::TextureCache::create_texture(fs::view("engine://resources/default/MRDAo")));
        material.set_param(SV_EMISSIVE, gfx::TextureCache::create_texture(fs::view("assets://textures/copper/copper-emissive-2048.png")));
        material.set_param(SV_HEIGHTSCALE, 1.f);
        material.set_param("discardExcess", false);
        material.set_param("skybox", gfx::TextureCache::create_texture("skyBox", fs::view("assets://textures/HDRI/earth.png")));

        auto ent = createEntity("Saturn");
        auto [pos, rot, scal] = ent.add_component<transform>();
        scal = scale(5.f, 5.f, 5.f);
        ent.add_component<gfx::mesh_renderer>(gfx::mesh_renderer(material, model));
        auto emitter = ent.add_component<particle_emitter>();
        emitter->infinite = true;
        emitter->spawnRate = 100;
        emitter->spawnInterval = 0.05f;
        emitter->maxSpawnCount = 100;
        emitter->minLifeTime = 5;
        emitter->maxLifeTime = 15;
        emitter->localScale = false;
        emitter->localPosition = false;

        emitter->add_policy<example_policy>();
        orbital_policy orbital;
        orbital.C_MASS = 100.f;
        orbital.G_FORCE = .1f;
        //emitter->add_policy<orbital_policy>(orbital);
        emitter->add_policy<gfx::rendering_policy>({ gfx::ModelCache::create_model("Quad", fs::view("assets://models/billboard.glb")), gfx::MaterialCache::create_material("Particle", fs::view("assets://shaders/particle.shs")) });
        gfx::MaterialCache::get_material("Particle").set_param("fixedSize", false);
    }

    {
        /* auto model = gfx::ModelCache::create_model("Sphere", fs::view("assets://models/sphere.obj"));
         auto material = gfx::MaterialCache::create_material("Water", fs::view("assets://shaders/light.shs"));
         material.set_param("color", math::color(0.f, 0.f, 1.f, 1.f));
         material.set_param("intensity", 1.f);*/

        auto ent = createEntity("Fountain");
        auto [pos, rot, scal] = ent.add_component<transform>();
        pos = position(0, 10, 0);
        auto emitter = ent.add_component<particle_emitter>();
        emitter->infinite = false;
        emitter->spawnRate = 10;
        emitter->spawnInterval = .05f;
        emitter->maxSpawnCount = 5000;
        emitter->minLifeTime = 1;
        emitter->maxLifeTime = 2;
        emitter->localScale = false;

        fountain_policy fountain;
        fountain.initForce = 20.f;
        emitter->add_policy<fountain_policy>(fountain);
        emitter->add_policy<scale_lifetime_policy>();
        auto mat = gfx::MaterialCache::create_material("Particle", fs::view("assets://shaders/particle.shs"));
        mat.set_param("particleSize", 1.f);
        mat.set_param("particleColor", math::color(1.f, 1.f, 1.f, 1.f));
        emitter->add_policy<gfx::rendering_policy>({ gfx::ModelCache::create_model("Quad", fs::view("assets://models/billboard.obj")), mat });
    }

    bindToEvent<events::exit, &ExampleSystem::onExit>();
}

