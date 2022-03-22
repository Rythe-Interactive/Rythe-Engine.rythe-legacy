#include "defaultpolicies.hpp"
#include <core/particles/particleemitter.hpp>
#include <core/particles/particlesystem.hpp>

namespace legion::core
{
#pragma region Example Policy
    void example_policy::onInit(particle_emitter& emitter, size_type start, size_type end)
    {
        static id_type posBufferId = nameHash("posBuffer");
        auto& posBuffer = emitter.has_buffer<position>(posBufferId) ? emitter.get_buffer<position>(posBufferId) : emitter.create_buffer<position>("posBuffer");

        for (size_type idx = start; idx < end; idx++)
        {
            auto baseDir = math::sphericalRand(1.f);
            baseDir.y = 0;
            int minBound = 5;
            int maxBound = 30;
            auto pos = math::normalize(baseDir) * minBound + math::normalize(baseDir) * (idx % (maxBound - minBound)) / 2.f;
            auto dist = math::length(pos);
            pos.y = math::sin(dist / math::pi<float>()) * 5.f * (pos.x / maxBound);
            posBuffer[idx] = pos;
        }
    }
#pragma endregion

#pragma region Orbital Policy
    void orbital_policy::setup(particle_emitter& emitter)
    {
        if (!emitter.has_uniform<float>("timeBuffer"))
            emitter.create_uniform<float>("timeBuffer") = 0.0f;
    }

    void orbital_policy::onInit(particle_emitter& emitter, size_type start, size_type end)
    {
        static id_type posBufferId = nameHash("posBuffer");
        static id_type velBufferId = nameHash("velBuffer");
        auto& posBuffer = emitter.has_buffer<position>(posBufferId) ? emitter.get_buffer<position>(posBufferId) : emitter.create_buffer<position>("posBuffer");
        auto& velBuffer = emitter.has_buffer<velocity>(velBufferId) ? emitter.get_buffer<velocity>(velBufferId) : emitter.create_buffer<velocity>("velBuffer");

        for (size_type idx = start; idx < end; idx++)
        {
            auto r2 = math::length2((math::vec3)posBuffer[idx]);
            auto force = G_FORCE * C_MASS / r2;
            auto r = math::length((math::vec3)posBuffer[idx]);
            auto speed = math::sqrt((G_FORCE * C_MASS) / r);
            velBuffer[idx] = math::normalize(math::cross(posBuffer[idx], math::vec3::up)) * speed;
        }
    }

    void orbital_policy::onUpdate(particle_emitter& emitter, float deltaTime, size_type count)
    {
        static id_type posBufferId = nameHash("posBuffer");
        static id_type velBufferId = nameHash("velBuffer");
        auto& posBuffer = emitter.get_buffer<position>("posBuffer");
        auto& velBuffer = emitter.get_buffer<velocity>("velBuffer");

        auto& timeBuffer = emitter.get_uniform<float>("timeBuffer");
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

#pragma endregion

#pragma region Fountain Policy
    void fountain_policy::onInit(particle_emitter& emitter, size_type start, size_type end)
    {
        static id_type posBufferId = nameHash("posBuffer");
        static id_type velBufferId = nameHash("velBuffer");
        auto& posBuffer = emitter.has_buffer<position>(posBufferId) ? emitter.get_buffer<position>(posBufferId) : emitter.create_buffer<position>("posBuffer");
        auto& velBuffer = emitter.has_buffer<velocity>(velBufferId) ? emitter.get_buffer<velocity>(velBufferId) : emitter.create_buffer<velocity>("velBuffer");

        for (size_type idx = start; idx < end; idx++)
        {
            posBuffer[idx] = math::vec3::zero;
            auto direction = math::vec3::up + math::normalize(math::vec3(math::linearRand(-5.f, 5.f), 0.f, math::linearRand(-5.f, 5.f)));
            velBuffer[idx] = direction * initForce;
        }
    }

    void fountain_policy::onUpdate(particle_emitter& emitter, float deltaTime, size_type count)
    {
        auto& posBuffer = emitter.get_buffer<position>("posBuffer");
        auto& velBuffer = emitter.get_buffer<velocity>("velBuffer");
        for (size_type idx = 0; idx < count; idx++)
        {
            posBuffer[idx] += velBuffer[idx] * deltaTime;
            velBuffer[idx] += math::vec3(0.f, -9.8f, 0.f) * deltaTime;
        }
    }
#pragma endregion

#pragma region Scale over Lifetime
    void scale_lifetime_policy::setup(particle_emitter& emitter)
    {
        if (!emitter.has_uniform<float>("scaleFactor"))
            emitter.create_uniform<float>("scaleFactor") = .3f;
    }

    void scale_lifetime_policy::onInit(particle_emitter& emitter, size_type start, size_type end)
    {
        static id_type scaleBufferId = nameHash("scaleBuffer");
        auto& scaleBuffer = emitter.has_buffer<scale>(scaleBufferId) ? emitter.get_buffer<scale>(scaleBufferId) : emitter.create_buffer<scale>("scaleBuffer");
        auto& ageBuffer = emitter.get_buffer<life_time>("lifetimeBuffer");
        auto scaleFactor = emitter.get_uniform<float>("scaleFactor");

        for (size_type idx = start; idx < end; idx++)
        {
            scaleBuffer[idx] = scale(scaleFactor) * math::linearRand(.1f, 1.f);
        }
    }

    void scale_lifetime_policy::onUpdate(particle_emitter& emitter, float deltaTime, size_type count)
    {
        auto& ageBuffer = emitter.get_buffer<life_time>("lifetimeBuffer");
        auto& scaleBuffer = emitter.get_buffer<scale>("scaleBuffer");

        auto scaleFactor = emitter.get_uniform<float>("scaleFactor");

        if (emitter.has_uniform<float>("minLifeTime") && emitter.has_uniform<float>("maxLifeTime"))
        {
            for (size_type idx = 0; idx < count; idx++)
            {
                scaleBuffer[idx] = scale(scaleFactor - ((ageBuffer[idx].age / ageBuffer[idx].max) * scaleFactor));
            }
        }
    }
#pragma endregion

#pragma region Boids
    void seperation_policy::setup(particle_emitter& emitter)
    {
        if (!emitter.has_uniform<bounds>("Bounds"))
            emitter.create_uniform<bounds>("Bounds", bounds{ position(-20.f),position(20.f) });
        if (!emitter.has_uniform<float>("visionRadius"))
            emitter.create_uniform<float>("visionRadius", 10.f);
        if (!emitter.has_uniform<float>("visionAngle"))
            emitter.create_uniform<float>("visionAngle", 45.f);
        if (!emitter.has_uniform<float>("speed"))
            emitter.create_uniform<float>("speed", 10.f);

        if (!emitter.has_buffer<velocity>("velBuffer"))
            emitter.create_buffer<velocity>("velBuffer");
        if (!emitter.has_buffer<position>("posBuffer"))
            emitter.create_buffer<position>("posBuffer");
        if (!emitter.has_buffer<rotation>("rotBuffer"))
            emitter.create_buffer<rotation>("rotBuffer");
        if (!emitter.has_buffer<std::vector<id_type>>("spatialGrid"))
            emitter.create_buffer<std::vector<id_type>>("spatialGrid");
    }
    void seperation_policy::onInit(particle_emitter& emitter, size_type start, size_type end)
    {
        auto& posBuffer = emitter.get_buffer<position>("posBuffer");
        auto& velBuffer = emitter.get_buffer<velocity>("velBuffer");
        auto& rotBuffer = emitter.get_buffer<rotation>("rotBuffer");
        auto& bnds = emitter.get_uniform<bounds>("Bounds");

        for (size_type idx = start; idx < end; idx++)
        {
            posBuffer[idx] = math::linearRand(bnds.min - math::vec3(-5.f), bnds.max - math::vec3(5.f));
            velBuffer[idx] = math::sphericalRand(1.f);
            rotBuffer[idx] = math::quatLookAt(velBuffer[idx], math::vec3::up);
        }
    }
    void seperation_policy::onUpdate(particle_emitter& emitter, float deltaTime, size_type count)
    {
        auto& posBuffer = emitter.get_buffer<position>("posBuffer");
        auto& velBuffer = emitter.get_buffer<velocity>("velBuffer");
        auto& rotBuffer = emitter.get_buffer<rotation>("rotBuffer");
        auto& bnds = emitter.get_uniform<bounds>("Bounds");
        auto& visionRadius = emitter.get_uniform<float>("visionRadius");
        auto& speed = emitter.get_uniform<float>("speed");

        auto& spatialGrid = emitter.get_buffer<std::vector<id_type>>("spatialGrid");

        for (size_type idx = 0; idx < count; idx++)
        {
            auto& pos = posBuffer[idx];
            spatialGrid[idx].clear();
            for (size_type idx2 = 0; idx2 < count; idx2++)
            {
                if (idx == idx2)
                    continue;
                if (math::length(posBuffer[idx2]-pos) < visionRadius)
                {
                    spatialGrid[idx].push_back(idx2);
                }
            }

            for (size_type neighbor = 0; neighbor < spatialGrid[idx].size(); neighbor++)
            {
                auto& neighborPos = posBuffer[spatialGrid[idx][neighbor]];
                auto diff = neighborPos - pos;
                velBuffer[idx] += diff * -(1.f / math::length2((math::vec3)diff));
            }

            velBuffer[idx] = math::normalize(velBuffer[idx]);
            rotBuffer[idx] = math::quatLookAt(velBuffer[idx], math::vec3::up);
            pos += velBuffer[idx] * speed * deltaTime;


            if (pos.x > bnds.max.x)
                pos.x = bnds.min.x;
            else if (pos.x < bnds.min.x)
                pos.x = bnds.max.x;

            if (pos.y > bnds.max.y)
                pos.y = bnds.min.y;
            else if (pos.y < bnds.min.y)
                pos.y = bnds.max.y;

            if (pos.z > bnds.max.z)
                pos.z = bnds.min.z;
            else if (pos.z < bnds.min.z)
                pos.z = bnds.max.z;
        }
    }

#pragma endregion
    void alignment_policy::setup(particle_emitter& emitter)
    {
        if (!emitter.has_uniform<bounds>("Bounds"))
            emitter.create_uniform<bounds>("Bounds", bounds{ position(-20.f),position(20.f) });
        if (!emitter.has_uniform<float>("visionRadius"))
            emitter.create_uniform<float>("visionRadius", 10.f);
        if (!emitter.has_uniform<float>("visionAngle"))
            emitter.create_uniform<float>("visionAngle", 45.f);
        if (!emitter.has_uniform<float>("speed"))
            emitter.create_uniform<float>("speed", 10.f);

        if (!emitter.has_buffer<velocity>("velBuffer"))
            emitter.create_buffer<velocity>("velBuffer");
        if (!emitter.has_buffer<position>("posBuffer"))
            emitter.create_buffer<position>("posBuffer");
    }
    void alignment_policy::onInit(particle_emitter& emitter, size_type start, size_type end)
    {
    }
    void alignment_policy::onUpdate(particle_emitter& emitter, float deltaTime, size_type count)
    {
    }
    void cohesion_policy::setup(particle_emitter& emitter)
    {
        if (!emitter.has_uniform<bounds>("Bounds"))
            emitter.create_uniform<bounds>("Bounds", bounds{ position(-20.f),position(20.f) });
        if (!emitter.has_uniform<float>("visionRadius"))
            emitter.create_uniform<float>("visionRadius", 10.f);
        if (!emitter.has_uniform<float>("visionAngle"))
            emitter.create_uniform<float>("visionAngle", 45.f);
        if (!emitter.has_uniform<float>("speed"))
            emitter.create_uniform<float>("speed", 10.f);

        if (!emitter.has_buffer<velocity>("velBuffer"))
            emitter.create_buffer<velocity>("velBuffer");
        if (!emitter.has_buffer<position>("posBuffer"))
            emitter.create_buffer<position>("posBuffer");
    }
    void cohesion_policy::onInit(particle_emitter& emitter, size_type start, size_type end)
    {
    }
    void cohesion_policy::onUpdate(particle_emitter& emitter, float deltaTime, size_type count)
    {
    }
}
