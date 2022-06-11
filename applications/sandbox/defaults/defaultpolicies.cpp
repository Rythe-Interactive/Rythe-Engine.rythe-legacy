#include "defaultpolicies.hpp"
#include <core/particles/particleemitter.hpp>
#include <core/particles/particlesystem.hpp>
#include <rendering/debugrendering.hpp>
#include <core/compute/compute.hpp>

namespace legion::core
{
#pragma region Example Policy
    void example_policy::onInit(particle_emitter& emitter, size_type start, size_type end)
    {
        static id_type posBufferId = nameHash("posBuffer");
        auto& posBuffer = emitter.has_buffer<math::vec4>(posBufferId) ? emitter.get_buffer<math::vec4>(posBufferId) : emitter.create_buffer<math::vec4>("posBuffer");

        for (size_type idx = start; idx < end; idx++)
        {
            auto baseDir = math::vec3(math::cos((idx % 3) * 60), 0.f, math::sin((idx % 3) * 60));
            baseDir.y = math::sin(idx);
            int minBound = 8;
            int maxBound = 60;
            auto pos = math::vec4(math::normalize(baseDir) * minBound + math::normalize(baseDir) * (idx % (maxBound - minBound)) / 2.f, 1);
            auto dist = math::length(pos);
            auto rand = math::diskRand(dist / maxBound);
            pos += math::vec4(rand.x, (1.f - (dist / maxBound)) * 2.f, rand.y, 0.f);
            //pos.y = math::sin(dist / math::pi<float>()) * 5.f * (pos.x / maxBound);
            posBuffer[idx] = pos;
        }
    }
#pragma endregion
#pragma region gpu_orbital_policy
    void gpu_orbital_policy::setup(particle_emitter& emitter)
    {
        if (!emitter.has_uniform<compute::function>("initPos"))
            emitter.create_uniform<compute::function>("initPos", fs::view("assets://kernels/orbitalPolicy.cl").load_as<compute::function>("init_pos")).setLocalSize(16);
        if (!emitter.has_uniform<compute::function>("initRot"))
            emitter.create_uniform<compute::function>("initRot", fs::view("assets://kernels/particles.cl").load_as<compute::function>("init_rot")).setLocalSize(16);
        if (!emitter.has_uniform<compute::function>("initScale"))
            emitter.create_uniform<compute::function>("initScale", fs::view("assets://kernels/particles.cl").load_as<compute::function>("init_scale")).setLocalSize(16);

        if (!emitter.has_uniform<compute::function>("initVel"))
            emitter.create_uniform<compute::function>("initVel", fs::view("assets://kernels/orbitalPolicy.cl").load_as<compute::function>("init_vel")).setLocalSize(16);
        if (!emitter.has_uniform<compute::function>("updateOrbits"))
            emitter.create_uniform<compute::function>("updateOrbits", fs::view("assets://kernels/orbitalPolicy.cl").load_as<compute::function>("update_orbits")).setLocalSize(16);

        if (!emitter.has_uniform<float>("gravForce"))
            emitter.create_uniform<float>("gravForce", 10.f);

        if (!emitter.has_uniform<float>("celestialMass"))
            emitter.create_uniform<float>("celestialMass", 100.f);

        if (!emitter.has_buffer<math::vec4>("posBuffer"))
            emitter.create_buffer<math::vec4>("posBuffer");
        if (!emitter.has_buffer<math::vec4>("velBuffer"))
            emitter.create_buffer<math::vec4>("velBuffer");
        if (!emitter.has_buffer<math::vec4>("rotBuffer"))
            emitter.create_buffer<math::vec4>("rotBuffer");
        if (!emitter.has_buffer<math::vec4>("scaleBuffer"))
            emitter.create_buffer<math::vec4>("scaleBuffer");
    }
    void gpu_orbital_policy::onInit(particle_emitter& emitter, size_type start, size_type end)
    {
        auto& init_position = emitter.get_uniform<compute::function>("initPos");
        auto& init_rotation = emitter.get_uniform<compute::function>("initRot");
        auto& init_scale = emitter.get_uniform<compute::function>("initScale");

        auto& init_vel = emitter.get_uniform<compute::function>("initVel");

        auto& posBuffer = emitter.get_buffer<math::vec4>("posBuffer");
        auto& velBuffer = emitter.get_buffer<math::vec4>("velBuffer");
        auto& rotBuffer = emitter.get_buffer<math::vec4>("rotBuffer");
        auto& scaleBuffer = emitter.get_buffer<math::vec4>("scaleBuffer");

        auto paddedSize = math::max(((end - start - 1) | 15) + 1, 16ull);
        int _start = static_cast<int>(start);
        int _end = static_cast<int>(end);

         //init_position(paddedSize, compute::inout(posBuffer, "positions"), compute::karg(_start, "start"), compute::karg(_end, "end"));

        rotation dir = rotation::lookat(math::vec3(0.f), math::vec3::forward);
        init_rotation(paddedSize, compute::inout(rotBuffer, "rotations"), compute::karg(dir, "direction"), compute::karg(_start, "start"), compute::karg(_end, "end"));

        math::vec4 scale = math::vec4(.25f);
        init_scale(paddedSize, compute::inout(scaleBuffer, "scales"), compute::karg(scale, "scale"), compute::karg(_start, "start"), compute::karg(_end, "end"));

        float gravForce = emitter.get_uniform<float>("gravForce");
        float c_mass = emitter.get_uniform<float>("celestialMass");
        init_vel(paddedSize, compute::inout(posBuffer, "positions"), compute::inout(velBuffer, "velocitys"), compute::karg(gravForce, "gravForce"), compute::karg(c_mass, "c_mass"), compute::karg(_start, "start"), compute::karg(_end, "end"));
    }
    void gpu_orbital_policy::onUpdate(particle_emitter& emitter, float deltaTime, size_type count)
    {
        using namespace lgn::core;
        if (count < 1)
            return;

        auto& update_orbits = emitter.get_uniform<compute::function>("updateOrbits");
        auto& init_position = emitter.get_uniform<compute::function>("initPos");
        auto& init_rotation = emitter.get_uniform<compute::function>("initRot");
        auto& init_scale = emitter.get_uniform<compute::function>("initScale");

        auto& posBuffer = emitter.get_buffer<math::vec4>("posBuffer");
        auto& velBuffer = emitter.get_buffer<math::vec4>("velBuffer");
        float gravForce = emitter.get_uniform<float>("gravForce");
        float c_mass = emitter.get_uniform<float>("celestialMass");
        auto livingBuffer = emitter.get_living_buffer();
        auto paddedSize = math::max(((count - 1) | 15) + 1, 16ull);

        //core::time::stopwatch watch;
        //watch.start();
        init_position.wait();
        init_rotation.wait();
        init_scale.wait();
        update_orbits.wait();
        update_orbits(paddedSize, compute::in(livingBuffer, "living"), compute::inout(posBuffer, "positions"), compute::inout(velBuffer, "velocitys"), compute::karg(gravForce, "gravForce"), compute::karg(c_mass, "c_mass"), compute::karg(deltaTime, "dt"));
        //watch.end();
        //log::debug("Orbital Calc update elapsed time {}ms", watch.elapsed_time().milliseconds());
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
#pragma region gpu_fountain_policy
    void gpu_fountain_policy::setup(particle_emitter& emitter)
    {
        if (!emitter.has_uniform<compute::function>("initParticle"))
            emitter.create_uniform<compute::function>("initParticle", fs::view("assets://kernels/particles.cl").load_as<compute::function>("init_particle")).setLocalSize(16);
        if (!emitter.has_uniform<compute::function>("initPos"))
            emitter.create_uniform<compute::function>("initPos", fs::view("assets://kernels/particles.cl").load_as<compute::function>("init_pos")).setLocalSize(16);
        if (!emitter.has_uniform<compute::function>("initRot"))
            emitter.create_uniform<compute::function>("initRot", fs::view("assets://kernels/particles.cl").load_as<compute::function>("init_rot")).setLocalSize(16);
        if (!emitter.has_uniform<compute::function>("initScale"))
            emitter.create_uniform<compute::function>("initScale", fs::view("assets://kernels/particles.cl").load_as<compute::function>("init_scale")).setLocalSize(16);

        if (!emitter.has_uniform<compute::function>("initVel"))
            emitter.create_uniform<compute::function>("initVel", fs::view("assets://kernels/fountainPolicy.cl").load_as<compute::function>("init_vel")).setLocalSize(16);

        if (!emitter.has_uniform<compute::function>("gravUpdate"))
            emitter.create_uniform<compute::function>("gravUpdate", fs::view("assets://kernels/fountainPolicy.cl").load_as<compute::function>("grav_update")).setLocalSize(16);

        if (!emitter.has_uniform<math::vec4>("gravity"))
            emitter.create_uniform<math::vec4>("gravity", math::vec4(0.f, -1.f, 0.f, 0.f));

        if (!emitter.has_buffer<math::vec4>("posBuffer"))
            emitter.create_buffer<math::vec4>("posBuffer");
        if (!emitter.has_buffer<math::vec4>("velBuffer"))
            emitter.create_buffer<math::vec4>("velBuffer");
        if (!emitter.has_buffer<math::vec4>("rotBuffer"))
            emitter.create_buffer<math::vec4>("rotBuffer");
        if (!emitter.has_buffer<math::vec4>("scaleBuffer"))
            emitter.create_buffer<math::vec4>("scaleBuffer");


        auto& init_particle = emitter.get_uniform<compute::function>("initParticle");
        init_particle.setAsyncDispatch(true);
        auto& init_vel = emitter.get_uniform<compute::function>("initVel");
        init_vel.setAsyncDispatch(true);
        auto& grav_update = emitter.get_uniform<compute::function>("gravUpdate");
        grav_update.setAsyncDispatch(true);
    }
    void gpu_fountain_policy::onInit(particle_emitter& emitter, size_type start, size_type end)
    {
        auto& init_particle = emitter.get_uniform<compute::function>("initParticle");
        auto& init_position = emitter.get_uniform<compute::function>("initPos");
        auto& init_rotation = emitter.get_uniform<compute::function>("initRot");
        auto& init_scale = emitter.get_uniform<compute::function>("initScale");

        auto& init_vel = emitter.get_uniform<compute::function>("initVel");

        auto& posBuffer = emitter.get_buffer<math::vec4>("posBuffer");
        auto& velBuffer = emitter.get_buffer<math::vec4>("velBuffer");
        auto& rotBuffer = emitter.get_buffer<math::vec4>("rotBuffer");
        auto& scaleBuffer = emitter.get_buffer<math::vec4>("scaleBuffer");

        auto paddedSize = math::max(((end - start - 1) | 15) + 1, 16ull);
        int _start = static_cast<int>(start);
        int _end = static_cast<int>(end);

        //core::time::stopwatch watch;
        //watch.start();
        math::vec4 initPosition = math::vec4(0.f);
        rotation dir = rotation::lookat(math::vec3(0.f), math::vec3::forward);
        math::vec4 scale = math::vec4(.1f);
        std::vector<math::vec4> initForce;
        for (size_type i = start; i < end; i++)
        {
            auto diskRand = math::diskRand(.5f);
            initForce.push_back(math::vec4(diskRand.x, 1.f, diskRand.y, 0.f) * 2.f);
        }
        init_particle(paddedSize, compute::inout(posBuffer, "positions"), compute::karg(initPosition, "position"),
            compute::inout(rotBuffer, "rotations"), compute::karg(dir, "direction"),
            compute::inout(scaleBuffer, "scales"), compute::karg(scale, "scale"),
            compute::inout(velBuffer, "velocitys"), compute::inout(initForce, "initForce"),
            compute::karg(_start, "start"), compute::karg(_end, "end"));
    }
    void gpu_fountain_policy::onUpdate(particle_emitter& emitter, float deltaTime, size_type count)
    {
        using namespace lgn::core;
        if (count < 1)
            return;

        auto& init_particle = emitter.get_uniform<compute::function>("initParticle");

        auto& posBuffer = emitter.get_buffer<math::vec4>("posBuffer");
        auto& velBuffer = emitter.get_buffer<math::vec4>("velBuffer");
        auto livingBuffer = emitter.get_living_buffer();
        auto paddedSize = math::max(((count - 1) | 15) + 1, 16ull);

        math::vec4 gravity = emitter.get_uniform<math::vec4>("gravity");
        auto& grav_update = emitter.get_uniform<compute::function>("gravUpdate");
        init_particle.wait();
        grav_update.wait();
        grav_update(paddedSize, compute::in(livingBuffer, "living"), compute::inout(posBuffer, "positions"), compute::inout(velBuffer, "velocitys"), compute::karg(gravity, "gravity"), compute::karg(deltaTime, "dt"));
    }
#pragma endregion
#pragma region Fountain Policy
    void fountain_policy::setup(particle_emitter& emitter)
    {
        if (!emitter.has_buffer<math::vec4>("posBuffer"))
            emitter.create_buffer<math::vec4>("posBuffer");
        if (!emitter.has_buffer<math::vec4>("velBuffer"))
            emitter.create_buffer<math::vec4>("velBuffer");
        if (!emitter.has_buffer<math::vec4>("rotBuffer"))
            emitter.create_buffer<math::vec4>("rotBuffer");
        if (!emitter.has_buffer<math::vec4>("scaleBuffer"))
            emitter.create_buffer<math::vec4>("scaleBuffer");
    }
    void fountain_policy::onInit(particle_emitter& emitter, size_type start, size_type end)
    {
        auto& posBuffer = emitter.get_buffer<math::vec4>("posBuffer");
        auto& velBuffer = emitter.get_buffer<math::vec4>("velBuffer");
        auto& rotBuffer = emitter.get_buffer<math::vec4>("rotBuffer");
        auto& scaleBuffer = emitter.get_buffer<math::vec4>("scaleBuffer");

        math::vec4 initPosition = math::vec4(0.f);
        math::vec4 dir = math::vec4(math::vec3::zero, 1.f);
        math::vec4 scale = math::vec4(.1f);
        for (size_type idx = start; idx < end; idx++)
        {
            posBuffer[idx] = initPosition;
            rotBuffer[idx] = dir;
            scaleBuffer[idx] = scale;

            auto diskRand = math::diskRand(1.0f);
            auto direction = math::vec4(diskRand.x, 1.f, diskRand.y, 0.f);
            velBuffer[idx] = direction * initForce;
        }
    }

    void fountain_policy::onUpdate(particle_emitter& emitter, float deltaTime, size_type count)
    {
        auto& posBuffer = emitter.get_buffer<math::vec4>("posBuffer");
        auto& velBuffer = emitter.get_buffer<math::vec4>("velBuffer");

        for (size_type idx = 0; idx < count; idx++)
        {
            velBuffer[idx] += gravity * deltaTime;
            posBuffer[idx] += velBuffer[idx] * deltaTime;
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
#pragma region color_life_time_policy
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
#pragma endregion
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
