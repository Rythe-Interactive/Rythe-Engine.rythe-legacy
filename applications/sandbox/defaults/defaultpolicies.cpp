#include "defaultpolicies.hpp"
#include <core/particles/particleemitter.hpp>
#include <core/particles/particlesystem.hpp>
#include <rendering/debugrendering.hpp>

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
            auto randpoint = math::diskRand(.2f);
            posBuffer[idx] = math::vec3(randpoint.x, math::linearRand(0.f, 0.8f), randpoint.y);
            auto direction = math::vec3::up /*+ math::normalize(math::vec3(math::linearRand(-1.f, 1.f), 0.f, math::linearRand(-1.f, 1.f)))*/;
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
            velBuffer[idx] += math::vec3(0.f, -0.10f, 0.f) * deltaTime;
        }
    }
#pragma endregion
#pragma region Scale over Lifetime
    void scale_lifetime_policy::setup(particle_emitter& emitter)
    {
        if (!emitter.has_uniform<float>("scaleFactor"))
            emitter.create_uniform<float>("scaleFactor") = .2f;
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
    void color_lifetime_policy::setup(particle_emitter& emitter)
    {
        if (!emitter.has_buffer<math::color>("colorBuffer"))
            emitter.create_buffer<math::color>("colorBuffer");

        if (!emitter.has_uniform<math::color>("startColor"))
            emitter.create_uniform<math::color>("startColor", math::colors::white);
        if (!emitter.has_uniform<math::color>("endColor"))
            emitter.create_uniform<math::color>("endColor", math::color(1.f, 1.f, 1.f, 0.f));
    }
    void color_lifetime_policy::onUpdate(particle_emitter& emitter, float deltaTime, size_type count)
    {
        auto& ageBuffer = emitter.get_buffer<life_time>("lifetimeBuffer");
        auto& colorBuffer = emitter.get_buffer<math::color>("colorBuffer");
        auto& startColor = emitter.get_uniform<math::color>("startColor");
        auto& endColor = emitter.get_uniform<math::color>("endColor");

        if (emitter.has_uniform<float>("minLifeTime") && emitter.has_uniform<float>("maxLifeTime"))
        {
            for (size_type idx = 0; idx < count; idx++)
            {
                colorBuffer[idx] = (startColor - (ageBuffer[idx].age / ageBuffer[idx].max) * startColor) + endColor;
            }
        }
    }

#pragma region Boids
#pragma region Locomotion
    void locomotion_policy::setup(particle_emitter& emitter)
    {
        if (!emitter.has_uniform<bounds>("Bounds"))
            emitter.create_uniform<bounds>("Bounds", bounds{ position(-40.f),position(40.f), 15.f });
        if (!emitter.has_uniform<float>("visionRadius"))
            emitter.create_uniform<float>("visionRadius", 5.f);
        if (!emitter.has_uniform<float>("seperationRadius"))
            emitter.create_uniform<float>("seperationRadius", 3.f);
        //if (!emitter.has_uniform<float>("visionAngle"))
        //    emitter.create_uniform<float>("visionAngle", 45.f);
        if (!emitter.has_uniform<float>("speed"))
            emitter.create_uniform<float>("speed", 2.f);

        if (!emitter.has_buffer<velocity>("velBuffer"))
            emitter.create_buffer<velocity>("velBuffer");
        if (!emitter.has_buffer<position>("posBuffer"))
            emitter.create_buffer<position>("posBuffer");
        if (!emitter.has_buffer<rotation>("rotBuffer"))
            emitter.create_buffer<rotation>("rotBuffer");
        if (!emitter.has_buffer<std::vector<id_type>>("spatialGrid"))
            emitter.create_buffer<std::vector<id_type>>("spatialGrid");
        if (!emitter.has_buffer<velocity>("steeringBuffer"))
            emitter.create_buffer<velocity>("steeringBuffer");
    }
    void locomotion_policy::onInit(particle_emitter& emitter, size_type start, size_type end)
    {
        auto& bnds = emitter.get_uniform<bounds>("Bounds");
        auto& speed = emitter.get_uniform<float>("speed");
        auto& posBuffer = emitter.get_buffer<position>("posBuffer");
        auto& velBuffer = emitter.get_buffer<velocity>("velBuffer");
        auto& steering = emitter.get_buffer<velocity>("steeringBuffer");

        for (size_type idx = start; idx < end; idx++)
        {
            posBuffer[idx] = math::linearRand(bnds.min + math::vec3(bnds.border), bnds.max - math::vec3(bnds.border));
            velBuffer[idx] = math::sphericalRand(1.0f) * speed;
            steering[idx] = velBuffer[idx];
        }
    }
    void locomotion_policy::onUpdate(particle_emitter& emitter, float deltaTime, size_type count)
    {
        auto& posBuffer = emitter.get_buffer<position>("posBuffer");
        auto& velBuffer = emitter.get_buffer<velocity>("velBuffer");
        auto& rotBuffer = emitter.get_buffer<rotation>("rotBuffer");
        auto& steeringBuffer = emitter.get_buffer<velocity>("steeringBuffer");
        auto& spatialGrid = emitter.get_buffer<std::vector<id_type>>("spatialGrid");

        auto& visionRadius = emitter.get_uniform<float>("visionRadius");
        auto& bnds = emitter.get_uniform<bounds>("Bounds");
        auto& speed = emitter.get_uniform<float>("speed");

        debug::drawCube(bnds.min, bnds.max);

        for (size_type idx = 0; idx < count; idx++)
        {
            auto& pos = posBuffer[idx];
            auto& vel = velBuffer[idx];
            auto& rot = rotBuffer[idx];
            auto& steering = steeringBuffer[idx];

            //Boundary Avoidance
            if (pos.x < bnds.min.x + bnds.border && velBuffer[idx].x < 0 && steering.x < 0)
                steering.x *= -1.f;
            if (pos.x > bnds.max.x - bnds.border && velBuffer[idx].x > 0 && steering.x > 0)
                steering.x *= -1.f;

            if (pos.y < bnds.min.y + bnds.border && velBuffer[idx].y < 0 && steering.y < 0)
                steering.y *= -1.f;
            if (pos.y > bnds.max.y - bnds.border && velBuffer[idx].y > 0 && steering.y > 0)
                steering.y *= -1.f;

            if (pos.z < bnds.min.z + bnds.border && velBuffer[idx].z < 0 && steering.z < 0)
                steering.z *= -1.f;
            if (pos.z > bnds.max.z - bnds.border && velBuffer[idx].z > 0 && steering.z > 0)
                steering.z *= -1.f;

            //Particle Integration
            auto randpoint = math::diskRand(math::atan(45.f));
            steering += math::normalize(vel + rot * math::vec3(randpoint.x, randpoint.y, 0.f)) * speed;
            vel = math::clamp(math::lerp(vel, steering, deltaTime), -velocity{ 5.f }, velocity{ 5.f });
            steering = math::normalize(vel);
            rot = math::quatLookAt(math::normalize(vel), math::vec3::up);
            pos += vel * speed * deltaTime;
            steering = vel;

            //Boundary Teleportation
            //if (pos.x < bnds.min.x)
            //    pos.x = bnds.max.x;
            //if (pos.x > bnds.max.x)
            //    pos.x = bnds.min.x;

            //if (pos.y < bnds.min.y)
            //    pos.y = bnds.max.y;
            //if (pos.y > bnds.max.y)
            //    pos.y = bnds.min.y;

            //if (pos.z < bnds.min.z)
            //    pos.z = bnds.max.z;
            //if (pos.z > bnds.max.z)
            //    pos.z = bnds.min.z;

            //Recalculates the spatial grid
            auto& spatial = spatialGrid[idx];
            spatial.clear();
            for (size_type idx2 = 0; idx2 < count; idx2++)
            {
                if (idx == idx2)
                    continue;

                if (math::length(posBuffer[idx2] - pos) < visionRadius)
                {
                    spatial.push_back(idx2);
                }
            }
        }
    }
#pragma endregion
#pragma region Alignment
    void alignment_policy::onUpdate(particle_emitter& emitter, float deltaTime, size_type count)
    {
        auto& posBuffer = emitter.get_buffer<position>("posBuffer");
        auto& velBuffer = emitter.get_buffer<velocity>("velBuffer");
        auto& steering = emitter.get_buffer<velocity>("steeringBuffer");
        auto& spatialGrid = emitter.get_buffer<std::vector<id_type>>("spatialGrid");

        for (size_type idx = 0; idx < count; idx++)
        {
            auto neighborHood = spatialGrid[idx];
            size_type neighborCount = neighborHood.size();
            if (neighborCount == 0)
                continue;

            velocity force{ 0.f };
            for (size_type neighbor = 0; neighbor < neighborCount; neighbor++)
                force += velBuffer[neighborHood[neighbor]];

            steering[idx] += force / neighborCount;
            steering[idx] -= velBuffer[idx];
        }
    }
#pragma endregion
#pragma region Cohesion
    void cohesion_policy::onUpdate(particle_emitter& emitter, float deltaTime, size_type count)
    {
        auto& posBuffer = emitter.get_buffer<position>("posBuffer");
        auto& velBuffer = emitter.get_buffer<velocity>("velBuffer");
        auto& steering = emitter.get_buffer<velocity>("steeringBuffer");
        auto& spatialGrid = emitter.get_buffer<std::vector<id_type>>("spatialGrid");

        //auto& speed = emitter.get_uniform<float>("speed");

        for (size_type idx = 0; idx < count; idx++)
        {
            auto neighborHood = spatialGrid[idx];
            size_type neighborCount = neighborHood.size();
            if (neighborCount == 0)
                continue;

            position sumPos{ 0.f };
            for (size_type neighbor = 0; neighbor < neighborCount; neighbor++)
            {
                sumPos += posBuffer[neighborHood[neighbor]];
            }
            auto avgPos = sumPos / neighborCount;
            steering[idx] += avgPos - posBuffer[idx];
            //steering[idx] -= velBuffer[idx];
        }
    }
#pragma endregion
#pragma region Seperation
    void seperation_policy::onUpdate(particle_emitter& emitter, float deltaTime, size_type count)
    {
        auto& posBuffer = emitter.get_buffer<position>("posBuffer");
        auto& velBuffer = emitter.get_buffer<velocity>("velBuffer");
        auto& steering = emitter.get_buffer<velocity>("steeringBuffer");
        auto& spatialGrid = emitter.get_buffer<std::vector<id_type>>("spatialGrid");

        auto& seperationRadius = emitter.get_uniform<float>("seperationRadius");

        for (size_type idx = 0; idx < count; idx++)
        {
            auto neighborHood = spatialGrid[idx];
            size_type neighborCount = neighborHood.size();
            if (neighborCount == 0)
                continue;

            auto& pos = posBuffer[idx];
            velocity force{ 0.f };
            for (size_type neighbor = 0; neighbor < neighborCount; neighbor++)
            {
                auto& neighborPos = posBuffer[neighborHood[neighbor]];
                auto diff = (pos - neighborPos).xyz();
                if (math::length(diff) < seperationRadius)
                    force += seperationRadius / diff;
            }
            steering[idx] += force / neighborCount;
            //steering[idx] -= velBuffer[idx];
        }
    }

#pragma endregion
#pragma endregion
}
