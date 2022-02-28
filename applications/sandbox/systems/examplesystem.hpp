#pragma once
#include <core/core.hpp>
#include <application/application.hpp>
#include <rendering/rendering.hpp>
#include <audio/audio.hpp>

struct example_comp
{

};

namespace legion::core
{
    struct example_policy : public particle_policy<example_policy>
    {
        NO_DTOR_RULE5_NOEXCEPT(example_policy);
        ~example_policy() = default;

        virtual void OnSetup(particle_emitter& emitter) override
        {

        }

        virtual void OnInit(particle_emitter& emitter, size_type start, size_type end) override
        {
            //log::debug("OnSetup");
            emitter.setUniform<std::string>("message", "goodbye world");

            for (size_type idx = start; idx <= end; idx++)
            {
                auto color = math::color(math::linearRand(0.f, 1.f), math::linearRand(0.f, 1.f), math::linearRand(0.f, 1.f), 1);
                emitter.getBuffer<math::color>()[idx] = color;
                emitter.getBuffer<scale>()[idx] = scale(1.f);
            }
        }

        virtual void OnUpdate(particle_emitter& emitter, float deltaTime, size_type count) override
        {
            auto& ageBuffer = emitter.getBuffer<life_time>();
            auto& scaleBuffer = emitter.getBuffer<scale>();
            auto& colorBuffer = emitter.getBuffer<math::color>();

            for (size_type idx = 0; idx < count; idx++)
            {
                float scaleFactor = math::clamp(ageBuffer[idx].max-ageBuffer[idx].age, 0.f, 1.f);
                scaleBuffer[idx] = scale(scaleFactor);
            }
        }

        virtual void OnDestroy(particle_emitter& emitter, size_type start, size_type end) override
        {
            log::debug(emitter.getUniform<std::string>("message").get());
        }
    };

    struct orbital_policy : public particle_policy<orbital_policy>
    {
        NO_DTOR_RULE5_NOEXCEPT(orbital_policy);
        ~orbital_policy() = default;

        const double C_MASS = 100.f;
        const double P_MASS = 50.f;
        const double G_FORCE = .1f;

        virtual void OnSetup(particle_emitter& emitter) override
        {

        }

        virtual void OnInit(particle_emitter& emitter, size_type start, size_type end) override
        {
            for (size_type idx = start; idx <= end; idx++)
            {
                emitter.getBuffer<position>()[idx] = math::normalize(math::sphericalRand(1.f)) * 3.f;
                emitter.getBuffer<velocity>()[idx] = math::normalize(math::cross(emitter.getBuffer<position>()[idx], math::vec3::up)) * 2.f;
            }
        }

        virtual void OnUpdate(particle_emitter& emitter, float deltaTime, size_type count) override
        {
            auto& posBuffer = emitter.getBuffer<position>();
            auto& velBuffer = emitter.getBuffer<velocity>();

            for (size_type idx = 0; idx < count; idx++)
            {
                auto r2 = math::pow(posBuffer[idx].x, 2) + math::pow(posBuffer[idx].y, 2) + math::pow(posBuffer[idx].z, 2);
                auto force = G_FORCE * ((P_MASS * C_MASS) / r2);
                velBuffer[idx] += math::normalize(-posBuffer[idx]) * (force / P_MASS) * deltaTime;
                posBuffer[idx] += velBuffer[idx] * deltaTime;
            }
        }

        virtual void OnDestroy(particle_emitter& emitter, size_type start, size_type end) override
        {

        }
    };

    struct rendering_policy : particle_policy<rendering_policy>
    {
        NO_DTOR_RULE5_NOEXCEPT(rendering_policy);
        ~rendering_policy() = default;

        virtual void OnSetup(particle_emitter& emitter) override
        {

        }

        virtual void OnInit(particle_emitter& emitter, size_type start, size_type end) override
        {
            auto model = gfx::ModelCache::create_model("Sphere", fs::view("assets://models/sphere.obj"));
            auto material = gfx::MaterialCache::create_material("Test", fs::view("assets://shaders/uv.shs"));

            emitter.setUniform<mesh_filter>("mesh_filter", mesh_filter(model.get_mesh()));
            emitter.setUniform<gfx::mesh_renderer>("renderer", gfx::mesh_renderer(material, model));
        }

        virtual void OnUpdate(particle_emitter& emitter, float deltaTime, size_type idx) override
        {

        }

        virtual void OnDestroy(particle_emitter& emitter, size_type start, size_type end) override
        {

        }
    };
}

class ExampleSystem final : public legion::System<ExampleSystem>
{
    lgn::size_type frames = 0;
    lgn::time64 totalTime = 0;
    lgn::time::stopwatch<lgn::time64> timer;
    std::array<lgn::time64, 18000> times;

public:
    void setup();

    void shutdown()
    {
        lgn::log::debug("ExampleSystem shutdown");
    }

    void onExit(L_MAYBEUNUSED lgn::events::exit& event)
    {
        using namespace legion;

        time64 avg0 = totalTime / frames;
        time64 avg1 = timer.elapsed_time() / frames;

        std::set<time64, std::greater<time64>> orderedTimes;

        for (auto& time : times)
            orderedTimes.insert(time);

        time64 onePcLow = 0;
        time64 pointOnePcLow = 0;

        size_type i = 0;
        for (auto& time : orderedTimes)
        {
            i++;
            onePcLow += time;

            if (i <= math::max<size_type>(math::uround(frames / 1000.0), 1))
            {
                pointOnePcLow += time;
            }

            if (i >= math::max<size_type>(math::uround(frames / 100.0), 1))
            {
                break;
            }
        }

        pointOnePcLow /= math::max<size_type>(math::uround(frames / 1000.0), 1);
        onePcLow /= math::max<size_type>(math::uround(frames / 100.0), 1);

        log::info("1%Low {:.3f} 0.1%Low {:.3f} Avg {:.3f} Measured Avg {:.3f}", onePcLow, pointOnePcLow, avg0, avg1);
        log::info("1%Low {:.3f} 0.1%Low {:.3f} Avg {:.3f} Measured Avg {:.3f}", 1.0 / onePcLow, 1.0 / pointOnePcLow, 1.0 / avg0, 1.0 / avg1);
    }

    void update(legion::time::span deltaTime)
    {
        using namespace legion;
        static bool firstFrame = true;
        if (firstFrame)
        {
            timer.start();
            firstFrame = false;
        }

        {
            ecs::filter<rotation, example_comp> filter;
            for (auto& ent : filter)
            {
                ent.get_component<rotation>().get() *= math::angleAxis(math::two_pi<float>() * 0.1f * deltaTime, math::vec3::up);
            }
        }

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

        time64 delta = schd::Clock::lastTickDuration();

        if (frames < times.size())
        {
            times[frames] = delta;
            frames++;
            totalTime += delta;
        }
        else
        {

            //raiseEvent<events::exit>();
        }
    }
};
