#include "../systems/examplesystem.hpp"
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

    auto model = gfx::ModelCache::create_model("Sphere", fs::view("assets://models/sphere.obj"));
    auto material = gfx::MaterialCache::create_material("Light", fs::view("assets://shaders/light.shs"));
    material.set_param("color", math::color(1.f, 0.f, 0.f, 1.f));
    material.set_param("intensity", 1.f);
    //material.set_param(SV_ALBEDO, gfx::TextureCache::create_texture(fs::view("assets://textures/detailedRock/Rock020_1K_Albedo.png")));
    //material.set_param(SV_NORMALHEIGHT, gfx::TextureCache::create_texture(fs::view("assets://textures/detailedRock/Rock020_1K_NormalHeight.png")));
    //material.set_param(SV_MRDAO, gfx::TextureCache::create_texture(fs::view("assets://textures/detailedRock/Rock020_1K_MRDAo.png")));
    //material.set_param(SV_EMISSIVE, gfx::TextureCache::create_texture(fs::view("assets://textures/detailedRock/Rock020_1K_emissive.png")));
    //material.set_param(SV_HEIGHTSCALE, 1.f);
    //material.set_param("discardExcess", false);
    //material.set_param("skycolor", math::color(0.1f, 0.3f, 1.0f));

    {
        auto ent = createEntity("Particle Emitter");
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
        //emitter->localScale = false;

        //emitter->add_policy<fountain_policy>();
        emitter->add_policy<example_policy>();
        emitter->add_policy<orbital_policy>();
        emitter->add_policy<rendering_policy>();
    }

    bindToEvent<events::exit, &ExampleSystem::onExit>();
}

namespace legion::core
{
#pragma region Example Policy
    void example_policy::OnSetup(particle_emitter& emitter)
    {
        emitter.create_uniform<float>("scaleFactor", .5f);
    }

    void example_policy::OnInit(particle_emitter& emitter, size_type start, size_type end)
    {
        auto scaleFactor = emitter.get_uniform<float>("scaleFactor");
        auto& scaleBuffer = emitter.get_buffer<scale>("scaleBuffer");
        auto& posBuffer = emitter.get_buffer<position>("posBuffer");

        for (size_type idx = start; idx <= end; idx++)
        {
            scaleBuffer[idx] = scale(scaleFactor);
            auto baseDir = math::sphericalRand(1.f);
            baseDir.y = 0;
            int minBound = 6;
            int maxBound = 9;
            auto pos = math::normalize(baseDir) * minBound + math::normalize(baseDir) * (idx % (maxBound - minBound));
            auto dist = math::length(pos);
            pos.y = math::sin(dist / math::pi<float>()) * 5.f * (pos.x / maxBound);
            posBuffer[idx] = pos;
        }
    }

    void  example_policy::OnUpdate(particle_emitter& emitter, float deltaTime, size_type count)
    {
        //auto& ageBuffer = emitter.getBuffer<life_time>("lifetimeBuffer");
        //auto& scaleBuffer = emitter.getBuffer<scale>("scaleBuffer");

        //auto scaleFactor = emitter.getUniform<float>("scaleFactor");

        //for (size_type idx = 0; idx < count; idx++)
        //{
        //    scaleBuffer[idx] = scale(scaleFactor - ((ageBuffer[idx].age / ageBuffer[idx].max) * scaleFactor));
        //}
    }

    void  example_policy::OnDestroy(particle_emitter& emitter, size_type start, size_type end)
    {
    }
#pragma endregion

#pragma region Orbital Policy
    void orbital_policy::OnSetup(particle_emitter& emitter)
    {
        emitter.create_uniform<float>("timeBuffer", 0.0f);
    }

    void orbital_policy::OnInit(particle_emitter& emitter, size_type start, size_type end)
    {
        auto& velBuffer = emitter.create_buffer<velocity>("velBuffer");
        auto& posBuffer = emitter.create_buffer<position>("posBuffer");
        for (size_type idx = start; idx <= end; idx++)
        {
            auto r2 = math::length2((math::vec3)posBuffer[idx]);
            auto force = G_FORCE * C_MASS / r2;
            auto r = math::length((math::vec3)posBuffer[idx]);
            auto speed = math::sqrt((G_FORCE * C_MASS) / r);
            velBuffer[idx] = math::normalize(math::cross(posBuffer[idx], math::vec3::up)) * speed;
        }
    }

    void orbital_policy::OnUpdate(particle_emitter& emitter, float deltaTime, size_type count)
    {
        auto& posBuffer = emitter.get_buffer<position>("posBuffer");
        auto& velBuffer = emitter.get_buffer<velocity>("velBuffer");
        auto& timeBuffer = emitter.create_uniform<float>("timeBuffer");
        timeBuffer += deltaTime;
        size_type iter = 0;
        while (timeBuffer > 0.02f)
        {
            schd::Scheduler::queueJobs(count, [&]()
                {
                    auto idx = async::this_job::get_id();
                    auto r2 = math::length2((math::vec3)posBuffer[idx]);
                    auto force = G_FORCE * C_MASS / r2;

                    velBuffer[idx] += -posBuffer[idx] * math::inversesqrt(r2) * force * 0.02f;
                    posBuffer[idx] += velBuffer[idx] * 0.02f;
                }).wait();
                timeBuffer -= 0.02f;
                iter++;
                if (iter > 2)
                    break;
        }
    }

    void orbital_policy::OnDestroy(particle_emitter& emitter, size_type start, size_type end)
    {

    }
#pragma endregion

#pragma region Rendering Policy
    void rendering_policy::OnSetup(particle_emitter& emitter)
    {
        auto model = gfx::ModelCache::create_model("Sphere", fs::view("assets://models/sphere.obj"));
        auto material = gfx::MaterialCache::create_material("Particle", fs::view("assets://shaders/pbr.shs"));
        //material.set_param("color", math::color(1.f, 1.f, 1.f, 1.f));
        material.set_param(SV_ALBEDO, gfx::TextureCache::create_texture(fs::view("assets://textures/aluminum-albedo.png")));
        material.set_param(SV_NORMALHEIGHT, gfx::TextureCache::create_texture(fs::view("assets://textures/aluminum-normalHeight.png")));
        material.set_param(SV_MRDAO, gfx::TextureCache::create_texture(fs::view("assets://textures/aluminum-MRDAo.png")));
        material.set_param(SV_EMISSIVE, gfx::TextureCache::create_texture(fs::view("assets://textures/aluminum-emissive.png")));
        material.set_param(SV_HEIGHTSCALE, 1.f);
        material.set_param("discardExcess", false);
        material.set_param("skycolor", math::color(0.1f, 0.3f, 1.0f));
        emitter.create_uniform<mesh_filter>("mesh_filter", mesh_filter(model.get_mesh()));
        emitter.create_uniform<gfx::mesh_renderer>("renderer", gfx::mesh_renderer(material, model));
    }

    void rendering_policy::OnInit(particle_emitter& emitter, size_type start, size_type end)
    {

    }

    void rendering_policy::OnUpdate(particle_emitter& emitter, float deltaTime, size_type count)
    {

    }

    void rendering_policy::OnDestroy(particle_emitter& emitter, size_type start, size_type end)
    {

    }
#pragma endregion

#pragma region Fountain Policy
    void fountain_policy::OnSetup(particle_emitter& emitter)
    {

    }

    void fountain_policy::OnInit(particle_emitter& emitter, size_type start, size_type end)
    {
        auto& posBuffer = emitter.create_buffer<position>("posBuffer");
        auto& velBuffer = emitter.create_buffer<velocity>("velBuffer");
        for (size_type idx = start; idx <= end; idx++)
        {
            posBuffer[idx] = math::vec3::zero;
            auto direction = math::vec3::up + math::normalize(math::vec3(math::linearRand(-5.f, 5.f), math::linearRand(-5.f, 5.f), math::linearRand(-5.f, 5.f)));
            velBuffer[idx] = direction * 2;
        }
    }

    void fountain_policy::OnUpdate(particle_emitter& emitter, float deltaTime, size_type count)
    {
        auto& posBuffer = emitter.get_buffer<position>("posBuffer");
        auto& velBuffer = emitter.get_buffer<velocity>("velBuffer");
        for (size_type idx = 0; idx < count; idx++)
        {
            posBuffer[idx] += velBuffer[idx] * deltaTime;
            velBuffer[idx] += math::vec3(0.f, -9.8f, 0.f) * deltaTime;
        }
    }

    void fountain_policy::OnDestroy(particle_emitter& emitter, size_type start, size_type end)
    {

    }
#pragma endregion
}
