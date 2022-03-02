#include "../systems/examplesystem.hpp"
namespace legion::core
{
    void example_policy::OnSetup(particle_emitter& emitter)
    {
        emitter.setUniform<float>("scaleFactor", .1f);
    }

    void  example_policy::OnInit(particle_emitter& emitter, size_type start, size_type end)
    {
        auto scaleFactor = emitter.getUniform<float>("scaleFactor").get();
        for (size_type idx = start; idx <= end; idx++)
        {
            emitter.getBuffer<scale>()[idx] = scale(scaleFactor);
            auto baseDir = math::sphericalRand(1.f);
            baseDir.y = 0;
            auto minBound = 4.f;
            auto maxBound = 15.f;
            auto pos = math::normalize(baseDir) * minBound + math::normalize(baseDir) * math::length(baseDir) * maxBound;
            auto dist = math::length(pos);
            auto theta = (dist / maxBound) * math::pi<float>();
            pos.y = math::sin(dist / 2.0f) * 5.0f * (pos.x / maxBound);
            emitter.getBuffer<position>()[idx] = pos;
        }
    }

    void  example_policy::OnUpdate(particle_emitter& emitter, float deltaTime, size_type count)
    {
        //auto& ageBuffer = emitter.getBuffer<life_time>();
        //auto& scaleBuffer = emitter.getBuffer<scale>();

        //auto scaleFactor = emitter.getUniform<float>("scaleFactor").get();

        //for (size_type idx = 0; idx < count; idx++)
        //{
        //    scaleBuffer[idx] = scale(scaleFactor - ((ageBuffer[idx].age / ageBuffer[idx].max) * scaleFactor));
        //}
    }

    void  example_policy::OnDestroy(particle_emitter& emitter, size_type start, size_type end)
    {
    }



    void orbital_policy::OnSetup(particle_emitter& emitter)
    {
        emitter.setUniform<float>("timeBuffer", 0.0f);
    }

    void orbital_policy::OnInit(particle_emitter& emitter, size_type start, size_type end)
    {
        auto& velBuffer = emitter.getBuffer<velocity>();
        auto& posBuffer = emitter.getBuffer<position>();
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
        auto& posBuffer = emitter.getBuffer<position>();
        auto& velBuffer = emitter.getBuffer<velocity>();
        auto& timeBuffer = emitter.getUniform<float>("timeBuffer").get();
        timeBuffer += deltaTime;
        size_type iter = 0;
        while (timeBuffer > 0.02f)
        {
            schd::Scheduler::queueJobs(count, [&]()
                {
                    auto idx = async::this_job::get_id();
                    auto r2 = math::length2((math::vec3)posBuffer[idx]);
                    auto force = G_FORCE * C_MASS / r2;
                    velBuffer[idx] += -posBuffer[idx] * math::inversesqrt(r2) * force * .02f;
                    posBuffer[idx] += velBuffer[idx] * .02f;
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

    void rendering_policy::OnSetup(particle_emitter& emitter)
    {
        auto model = gfx::ModelCache::create_model("Sphere", fs::view("assets://models/sphere.obj"));
        auto material = gfx::MaterialCache::create_material("Particle", fs::view("assets://shaders/worldposition.shs"));
        //material.set_param("color", math::color(1.f, 1.f, 1.f, 1.f));
        //material.set_param(SV_ALBEDO, gfx::TextureCache::create_texture(fs::view("engine://resources/default/albedo")));
        //material.set_param(SV_NORMALHEIGHT, gfx::TextureCache::create_texture(fs::view("engine://resources/default/normalHeight")));
        //material.set_param(SV_MRDAO, gfx::TextureCache::create_texture(fs::view("engine://resources/default/MRDAo")));
        //material.set_param(SV_EMISSIVE, gfx::TextureCache::create_texture(fs::view("engine://resources/default/emissive")));
        //material.set_param(SV_HEIGHTSCALE, 1.f);
        //material.set_param("discardExcess", false);
        //material.set_param("skycolor", math::color(0.1f, 0.3f, 1.0f));
        emitter.setUniform<mesh_filter>("mesh_filter", mesh_filter(model.get_mesh()));
        emitter.setUniform<gfx::mesh_renderer>("renderer", gfx::mesh_renderer(material, model));
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

    void fountain_policy::OnSetup(particle_emitter& emitter)
    {

    }

    void fountain_policy::OnInit(particle_emitter& emitter, size_type start, size_type end)
    {
        for (size_type idx = start; idx <= end; idx++)
        {
            emitter.getBuffer<position>()[idx] = math::vec3::zero;
            auto direction = math::vec3::up + math::normalize(math::vec3(math::linearRand(-5.f, 5.f), math::linearRand(-5.f, 5.f), math::linearRand(-5.f, 5.f)));
            emitter.getBuffer<velocity>()[idx] = direction * 2;
        }
    }

    void fountain_policy::OnUpdate(particle_emitter& emitter, float deltaTime, size_type count)
    {
        auto& posBuffer = emitter.getBuffer<position>();
        auto& velBuffer = emitter.getBuffer<velocity>();
        for (size_type idx = 0; idx < count; idx++)
        {
            posBuffer[idx] += velBuffer[idx] * deltaTime;
            velBuffer[idx] += math::vec3(0.f, -9.8f, 0.f) * deltaTime;
        }
    }

    void fountain_policy::OnDestroy(particle_emitter& emitter, size_type start, size_type end)
    {

    }
}
