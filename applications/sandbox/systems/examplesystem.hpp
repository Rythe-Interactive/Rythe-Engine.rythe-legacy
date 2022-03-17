#pragma once
#include <core/core.hpp>
#include <application/application.hpp>
#include <rendering/rendering.hpp>
#include <audio/audio.hpp>

#include <core/ecs/handles/entity.hpp>

namespace legion::core
{
    struct [[lgn::reflectable]] example_comp
    {
        int value = 1;
    };
}

class ExampleSystem final : public legion::System<ExampleSystem>
{
public:
    void setup()
    {
        using namespace legion;
        log::filter(log::severity_debug);
        log::debug("ExampleSystem setup");

        app::window& win = ecs::world.get_component<app::window>();
        app::context_guard guard(win);

        //auto model = gfx::ModelCache::create_model("Sphere", fs::view("assets://models/sphere.obj"));

        //auto material = gfx::MaterialCache::create_material("Default", fs::view("assets://shaders/pbr.shs"));
        //material.set_param(SV_ALBEDO, gfx::TextureCache::create_texture(fs::view("engine://resources/default/albedo")));
        //material.set_param(SV_NORMALHEIGHT, gfx::TextureCache::create_texture(fs::view("engine://resources/default/normalHeight")));
        //material.set_param(SV_MRDAO, gfx::TextureCache::create_texture(fs::view("engine://resources/default/MRDAo")));
        //material.set_param(SV_EMISSIVE, gfx::TextureCache::create_texture(fs::view("engine://resources/default/emissive")));
        //material.set_param(SV_HEIGHTSCALE, 1.f);
        //material.set_param("discardExcess", false);
        //material.set_param("skycolor", math::color(0.1f, 0.3f, 1.0f));

        //auto material = gfx::MaterialCache::create_material("Default", fs::view("assets://shaders/texture.shs"));
        //material.set_param("_texture", gfx::TextureCache::create_texture(fs::view("engine://resources/default/albedo")));

       //auto audioSegment = audio::AudioSegmentCache::createAudioSegment("Beep", fs::view("assets://audio/beep4.mp3"));

        {
            auto ent = createEntity("Sun");
            ent.add_component(gfx::light::directional(math::color(1, 1, 0.8f), 10.f));
            auto [pos, rot, scal] = ent.add_component<transform>();
            rot = rotation::lookat(math::vec3::zero, math::vec3(-1, -1, -1));
        }

        //Serialization Test
        srl::SerializerRegistry::registerSerializer<example_comp>();
        srl::SerializerRegistry::registerSerializer<ecs::entity>();
        srl::SerializerRegistry::registerSerializer<position>();
        srl::SerializerRegistry::registerSerializer<rotation>();
        srl::SerializerRegistry::registerSerializer<velocity>();
        srl::SerializerRegistry::registerSerializer<scale>();

        auto rootEnt = createEntity();
        rootEnt->name = "Root";
        auto comp = rootEnt.add_component<example_comp>();

        for (int i = 0; i < 3; i++)
        {
            auto child = createEntity();
            rootEnt.add_child(child);
            for (int j = 0; j < 2; j++)
            {
                auto child2 = createEntity();
                child.add_child(child2);
                if (j == 0)
                    child2.add_component(example_comp{ i });
                if (i == 0)
                    child2.add_component<velocity>();
                if (i == 1)
                    child2.add_component<rotation>();
            }
            if (i == 0)
                rootEnt.add_component<position>();
        }

        srl::write<srl::yaml>(fs::view("assets://scenes/scene1.yaml"), rootEnt, "scene");
        srl::write<srl::json>(fs::view("assets://scenes/scene1.json"), rootEnt, "scene");
        srl::write<srl::bson>(fs::view("assets://scenes/scene1.bson"), rootEnt, "scene");

        ecs::Registry::destroyEntity(rootEnt);

        auto result4 = srl::load<srl::bson, ecs::entity>(fs::view("assets://scenes/scene1.bson"), "scene");

        srl::write<srl::yaml>(fs::view("assets://scenes/scene2.yaml"), *result4, "scene");
        srl::write<srl::json>(fs::view("assets://scenes/scene2.json"), *result4, "scene");
        srl::write<srl::bson>(fs::view("assets://scenes/scene2.bson"), *result4, "scene");

        /////////////////////////////////////////////////////////
    }

    void update(legion::time::span deltaTime)
    {
        using namespace legion;

        ecs::filter<position, velocity, example_comp> filter;

        float dt = deltaTime;
        if (dt > 0.07f)
            return;

        if (filter.size())
        {
            auto poolSize = (schd::Scheduler::jobPoolSize() + 1);
            size_type jobSize = math::iround(math::ceil(filter.size() / static_cast<float>(poolSize)));

            queueJobs(poolSize, [&](id_type jobId)
                {
                    auto start = jobId * jobSize;
                    auto end = start + jobSize;
                    if (end > filter.size())
                        end = filter.size();

                    for (size_type i = start; i < end; i++)
                    {
                        auto& pos = filter[i].get_component<position>().get();
                        auto& vel = filter[i].get_component<velocity>().get();

                        if (vel == math::vec3::zero)
                            vel = math::normalize(pos);

                        math::vec3 perp;

                        perp = math::normalize(math::cross(vel, math::vec3::up));

                        math::vec3 rotated = (math::axisAngleMatrix(vel, math::perlin(pos) * math::pi<float>()) * math::vec4(perp.x, perp.y, perp.z, 0)).xyz();
                        rotated.y -= 0.5f;
                        rotated = math::normalize(rotated);

                        vel = math::normalize(vel + rotated * dt);

                        if (math::abs(vel.y) >= 0.9f)
                        {
                            auto rand = math::circularRand(1.f);
                            vel.y = 0.9f;
                            vel = math::normalize(vel + math::vec3(rand.x, 0.f, rand.y));
                        }

                        pos += vel * 0.3f * dt;
                    }
                }
            ).wait();
        }

        static fast_time buffer = 0;
        static fast_time avgTime = deltaTime;
        avgTime = (avgTime + deltaTime) / 2.f;
        buffer += deltaTime;

        if (buffer >= 1.f)
        {
            buffer--;
            log::debug("dt: {}s fps: {}", avgTime, 1.f / avgTime);
        }
    }
};
