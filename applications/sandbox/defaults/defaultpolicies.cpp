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
            auto baseDir = math::sphericalRand(1.f);
            baseDir.y = 0;
            int minBound = 5;
            int maxBound = 30;
            auto pos = math::vec4(math::normalize(baseDir) * minBound + math::normalize(baseDir) * (idx % (maxBound - minBound)) / 2.f, 1);
            auto dist = math::length(pos);
            pos.y = math::sin(dist / math::pi<float>()) * 5.f * (pos.x / maxBound);
            posBuffer[idx] = pos;
        }
    }
#pragma endregion
#pragma region Verlet Policy
    void verlet_integration_policy::setup(particle_emitter& emitter)
    {
        if (!emitter.has_uniform<compute::function>("verletIntegration"))
            emitter.create_uniform<compute::function>("verletIntegration", fs::view("assets://kernels/particles.cl").load_as<compute::function>("verlet_integration"));
        if (!emitter.has_uniform<compute::function>("constrainPositions"))
            emitter.create_uniform<compute::function>("constrainPositions", fs::view("assets://kernels/particles.cl").load_as<compute::function>("constrain_positions"));
        if (!emitter.has_uniform<compute::function>("resetSpatialGrid"))
            emitter.create_uniform<compute::function>("resetSpatialGrid", fs::view("assets://kernels/particles.cl").load_as<compute::function>("reset_spatial_grid"));
        if (!emitter.has_uniform<compute::function>("spatialGrid"))
            emitter.create_uniform<compute::function>("spatialGrid", fs::view("assets://kernels/particles.cl").load_as<compute::function>("spatial_grid"));
        if (!emitter.has_uniform<compute::function>("collisionDetection"))
            emitter.create_uniform<compute::function>("collisionDetection", fs::view("assets://kernels/particles.cl").load_as<compute::function>("collision_detection"));
        if (!emitter.has_uniform<compute::function>("initPos"))
            emitter.create_uniform<compute::function>("initPos", fs::view("assets://kernels/particles.cl").load_as<compute::function>("init_pos"));
        if (!emitter.has_uniform<compute::function>("initRot"))
            emitter.create_uniform<compute::function>("initRot", fs::view("assets://kernels/particles.cl").load_as<compute::function>("init_rot"));
        if (!emitter.has_uniform<compute::function>("initScale"))
            emitter.create_uniform<compute::function>("initScale", fs::view("assets://kernels/particles.cl").load_as<compute::function>("init_scale"));

        if (!emitter.has_buffer<math::vec4>("posBuffer"))
            emitter.create_buffer<math::vec4>("posBuffer");
        if (!emitter.has_buffer<math::vec4>("prevPosBuffer"))
            emitter.create_buffer<math::vec4>("prevPosBuffer");
        if (!emitter.has_buffer<math::vec4>("rotBuffer"))
            emitter.create_buffer<math::vec4>("rotBuffer");
        if (!emitter.has_buffer<math::vec4>("scaleBuffer"))
            emitter.create_buffer<math::vec4>("scaleBuffer");

        if (!emitter.has_buffer<size_type>("gridCells"))
            emitter.create_buffer<size_type>("gridCells");

        auto& init_position = emitter.get_uniform<compute::function>("initPos");
        init_position.setLocalSize(16);

        auto& init_rotation = emitter.get_uniform<compute::function>("initRot");
        init_rotation.setLocalSize(16);

        auto& init_scale = emitter.get_uniform<compute::function>("initScale");
        init_scale.setLocalSize(16);

        auto& verlet_integration = emitter.get_uniform<compute::function>("verletIntegration");
        verlet_integration.setLocalSize(16);

        auto& constrain_positions = emitter.get_uniform<compute::function>("constrainPositions");
        constrain_positions.setLocalSize(16);

        auto& spatial_grid = emitter.get_uniform<compute::function>("spatialGrid");
        spatial_grid.setLocalSize(16);

        auto& collision_detection = emitter.get_uniform<compute::function>("collisionDetection");
        collision_detection.setLocalSize(16);

        auto& reset_spatial_grid = emitter.get_uniform<compute::function>("resetSpatialGrid");
        reset_spatial_grid.setLocalSize(16);
    }
    void verlet_integration_policy::onInit(particle_emitter& emitter, size_type start, size_type end)
    {
        auto& init_position = emitter.get_uniform<compute::function>("initPos");
        auto& init_rotation = emitter.get_uniform<compute::function>("initRot");
        auto& init_scale = emitter.get_uniform<compute::function>("initScale");
        auto& verlet_integration = emitter.get_uniform<compute::function>("verletIntegration");
        auto& constrain_positions = emitter.get_uniform<compute::function>("constrainPositions");

        auto& posBuffer = emitter.get_buffer<math::vec4>("posBuffer");
        auto& prevPosBuffer = emitter.get_buffer<math::vec4>("prevPosBuffer");
        auto& rotBuffer = emitter.get_buffer<math::vec4>("rotBuffer");
        auto& scaleBuffer = emitter.get_buffer<math::vec4>("scaleBuffer");

        auto count = end - start;

        auto paddedSize = math::max(((count - 1) | 15) + 1, 16ull);

        math::vec4 startPos = math::vec4(1.f, 0.f, 0.f, 0.f);
        math::vec4 prevPos = startPos;

        //auto ioPosBuffer = compute::Context::createBuffer(posBuffer,compute::buffer_type::READ_BUFFER | compute::buffer_type::WRITE_BUFFER, "positions");

        //std::vector<math::vec4> subPosBuffer = { posBuffer.begin() + start, posBuffer.begin() + end };
        //std::vector<math::vec4> subPrevPosBuffer = { prevPosBuffer.begin() + start, prevPosBuffer.begin() +end};

        //std::vector<math::vec4> subRotBuffer = { rotBuffer.begin() + start, rotBuffer.begin() + end };

        //std::vector<math::vec4> subScaleBuffer = { scaleBuffer.begin() + start, scaleBuffer.begin() + end };

        int _start = static_cast<int>(start);
        int _end = static_cast<int>(end);

        init_position(paddedSize, compute::inout(posBuffer, "positions"), compute::karg(startPos, "position"), compute::karg(_start, "start"), compute::karg(_end, "end"));
        init_position(paddedSize, compute::inout(prevPosBuffer, "positions"), compute::karg(prevPos, "position"), compute::karg(_start, "start"), compute::karg(_end, "end"));

        rotation dir = rotation::lookat(math::vec3(0.f), math::vec3::forward);
        init_rotation(paddedSize, compute::inout(rotBuffer, "rotations"), compute::karg(dir, "direction"), compute::karg(_start, "start"), compute::karg(_end, "end"));

        math::vec4 scale = math::vec4(1.f);
        init_scale(paddedSize, compute::inout(scaleBuffer, "scales"), compute::karg(scale, "scale"), compute::karg(_start, "start"), compute::karg(_end, "end"));
    }
    void verlet_integration_policy::onUpdate(particle_emitter& emitter, float deltaTime, size_type count)
    {
        using namespace lgn::core;
        if (count < 1)
            return;

        //core::time::stopwatch watch;
        //watch.start();

        auto& posBuffer = emitter.get_buffer<math::vec4>("posBuffer");
        auto& prevPosBuffer = emitter.get_buffer<math::vec4>("prevPosBuffer");
        auto& gridCells = emitter.get_buffer<size_type>("gridCells");
        auto livingBuffer = emitter.get_living_buffer();

        auto& spatial_grid = emitter.get_uniform<compute::function>("spatialGrid");
        auto paddedSize = math::max(((count - 1) | 15) + 1, 16ull);

        auto& reset_spatial_grid = emitter.get_uniform<compute::function>("resetSpatialGrid");
        reset_spatial_grid(math::max(((emitter.capacity() - 1) | 15) + 1, 16ull), compute::inout(gridCells, "gridCells"));

        math::vec4 cellDims(.1f);
        math::vec4 bounds(100.f);
        spatial_grid(paddedSize, compute::inout(gridCells, "gridCells"), compute::in(posBuffer, "positions"), compute::karg(cellDims, "cellDims"), compute::karg(bounds, "bounds"));

        auto& collision_detection = emitter.get_uniform<compute::function>("collisionDetection");
        collision_detection(paddedSize, compute::in(gridCells, "gridCells"), compute::inout(posBuffer, "positions"), compute::karg(bounds, "bounds"));

        math::vec4 acceleration = math::vec4(0.f, -1.f, 0.f, 0.f);
        auto& verlet_integration = emitter.get_uniform<compute::function>("verletIntegration");
        verlet_integration(paddedSize, compute::in(livingBuffer, "living"), compute::inout(posBuffer, "positions"), compute::inout(prevPosBuffer, "prevPositions"), compute::karg(acceleration, "acceleration"), compute::karg(deltaTime, "dt"));

        cl_float radius = 3.f;
        auto& constrain_positions = emitter.get_uniform<compute::function>("constrainPositions");
        constrain_positions(paddedSize, compute::in(livingBuffer, "living"), compute::inout(posBuffer, "positions"), compute::karg(radius, "radius"));

        /*watch.end();
        log::debug("OpenCL position update elapsed time: {}ms",watch.elapsed_time().milliseconds());*/
    }
#pragma endregion
#pragma region Point Cloud Policy
    void gpu_particle_policy::setup(particle_emitter& emitter)
    {
        if (!emitter.has_uniform<compute::function>("vadd"))
            emitter.create_uniform<compute::function>("vadd", fs::view("assets://kernels/particles.cl").load_as<compute::function>("vector_add"));
        if (!emitter.has_uniform<compute::function>("initVel"))
            emitter.create_uniform<compute::function>("initVel", fs::view("assets://kernels/particles.cl").load_as<compute::function>("init_vel"));
        if (!emitter.has_uniform<compute::function>("initVelField"))
            emitter.create_uniform<compute::function>("initVelField", fs::view("assets://kernels/particles.cl").load_as<compute::function>("init_velField"));
        if (!emitter.has_uniform<compute::function>("initPos"))
            emitter.create_uniform<compute::function>("initPos", fs::view("assets://kernels/particles.cl").load_as<compute::function>("init_pos"));
        if (!emitter.has_uniform<compute::function>("initPosCube"))
            emitter.create_uniform<compute::function>("initPosCube", fs::view("assets://kernels/particles.cl").load_as<compute::function>("init_pos_cube"));
        if (!emitter.has_uniform<compute::function>("initPosDisk"))
            emitter.create_uniform<compute::function>("initPosDisk", fs::view("assets://kernels/particles.cl").load_as<compute::function>("init_pos_disk"));
        if (!emitter.has_uniform<compute::function>("initRot"))
            emitter.create_uniform<compute::function>("initRot", fs::view("assets://kernels/particles.cl").load_as<compute::function>("init_rot"));
        if (!emitter.has_uniform<compute::function>("initScale"))
            emitter.create_uniform<compute::function>("initScale", fs::view("assets://kernels/particles.cl").load_as<compute::function>("init_scale"));

        if (!emitter.has_buffer<math::vec4>("posBuffer"))
            emitter.create_buffer<math::vec4>("posBuffer");
        if (!emitter.has_buffer<math::vec4>("rotBuffer"))
            emitter.create_buffer<math::vec4>("rotBuffer");
        if (!emitter.has_buffer<math::vec4>("scaleBuffer"))
            emitter.create_buffer<math::vec4>("scaleBuffer");

        if (!emitter.has_buffer<math::vec4>("velBuffer"))
            emitter.create_buffer<math::vec4>("velBuffer");

        auto& init_velocity = emitter.get_uniform<compute::function>("initVel");
        init_velocity.setLocalSize(16);

        auto& init_velField = emitter.get_uniform<compute::function>("initVelField");
        init_velField.setLocalSize(16);

        auto& init_position = emitter.get_uniform<compute::function>("initPos");
        init_position.setLocalSize(16);

        auto& init_pos_cube = emitter.get_uniform<compute::function>("initPosCube");
        init_pos_cube.setLocalSize(16);

        auto& init_pos_disk = emitter.get_uniform<compute::function>("initPosDisk");
        init_pos_disk.setLocalSize(16);

        auto& init_rotation = emitter.get_uniform<compute::function>("initRot");
        init_rotation.setLocalSize(16);

        auto& init_scale = emitter.get_uniform<compute::function>("initScale");
        init_scale.setLocalSize(16);

        auto& vector_add = emitter.get_uniform<compute::function>("vadd");
        vector_add.setLocalSize(16);

        auto& velBuffer = emitter.get_buffer<math::vec4>("velBuffer");
        auto& posBuffer = emitter.get_buffer<math::vec4>("posBuffer");
        auto& rotBuffer = emitter.get_buffer<math::vec4>("rotBuffer");
        auto& scaleBuffer = emitter.get_buffer<math::vec4>("scaleBuffer");

        auto paddedSize = math::max(((emitter.capacity() - 1) | 255) + 1, 256ull);

        //std::vector<math::vec4> directions;
        //for (size_type i = 0; i < 25; i++)
        //{
        //    directions.push_back(math::vec4(math::sphericalRand(1.f), 0.f));
        //}
        //cl_int dirCount = directions.size();
        cl_int radius = 3.f;
        //init_position(paddedSize, compute::inout(posBuffer, "positions"), compute::in(directions, "spawnDirs"), compute::karg(dirCount, "dirCount"), compute::karg(radius, "radius"));
        init_pos_disk(paddedSize, compute::inout(posBuffer, "positions"), compute::karg(radius, "radius"));

        math::vec4 center = math::vec4(0.f);
        init_velField(paddedSize, compute::inout(posBuffer, "positions"), compute::inout(velBuffer, "vectorField"), compute::karg(center, "center"));

        //init_pos_cube(paddedSize, compute::inout(posBuffer, "A"), compute::karg(width, "width"), compute::karg(height, "height"), compute::karg(depth, "depth"));

        //init_position(paddedSize, compute::inout(posBuffer, "A"));

        rotation dir = rotation::lookat(math::vec3(0.f), math::vec3::forward);
        init_rotation(paddedSize, compute::inout(rotBuffer, "A"), compute::karg(dir, "direction"));

        init_scale(paddedSize, compute::inout(scaleBuffer, "A"));
    }
    void gpu_particle_policy::onInit(particle_emitter& emitter, size_type start, size_type end)
    {
        //auto& velBuffer = emitter.get_buffer<math::vec4>("velBuffer");
        //auto& posBuffer = emitter.get_buffer<math::vec4>("posBuffer");
        //auto& rotBuffer = emitter.get_buffer<math::vec4>("rotBuffer");
        //auto& scaleBuffer = emitter.get_buffer<math::vec4>("scaleBuffer");

        //auto count = static_cast<cl_ulong>(end - start);
        //if (count < 1)
        //    return;

        //auto paddedSize = math::max(((emitter.capacity() - 1) | 15) + 1, 16ull);

        //auto _start = static_cast<cl_ulong>(start);

        //auto& init_velocity = emitter.get_uniform<compute::function>("initVel");
        //std::vector<math::vec4> directions;
        //for (size_type i = 0; i < paddedSize; i++)
        //{
        //    directions.push_back(math::vec4(math::ballRand(1.f), 0.f));
        //}
        //init_velocity(paddedSize, compute::inout(velBuffer, "A"), compute::in(directions, "directions"), compute::karg(_start, "start"), compute::karg(count, "count"));

        //auto& init_position = emitter.get_uniform<compute::function>("initPos");
        //init_position(paddedSize, compute::inout(posBuffer, "A"), compute::karg(_start, "start"), compute::karg(count, "count"));

        //rotation dir = rotation::lookat(math::vec3(0.f), math::vec3::forward);
        //auto& init_rotation = emitter.get_uniform<compute::function>("initRot");
        //init_rotation(paddedSize, compute::inout(rotBuffer, "A"), compute::karg(dir, "direction"), compute::karg(_start, "start"), compute::karg(count, "count"));

        //auto& init_scale = emitter.get_uniform<compute::function>("initScale");
        //init_scale(paddedSize, compute::inout(scaleBuffer, "A"), compute::karg(_start, "start"), compute::karg(count, "count"));

    }
    void gpu_particle_policy::onUpdate(particle_emitter& emitter, float deltaTime, size_type count)
    {
        using namespace lgn::core;
        if (count < 1)
            return;

        //core::time::stopwatch watch;
        //watch.start();

        auto& posBuffer = emitter.get_buffer<math::vec4>("posBuffer");
        auto& velBuffer = emitter.get_buffer<math::vec4>("velBuffer");

        auto paddedSize = math::max(((count - 1) | 512) + 1, 512ull);

        math::vec4 center = math::vec4(0.f);

        auto& vector_add = emitter.get_uniform<compute::function>("vadd");
        vector_add(paddedSize, compute::inout(posBuffer), compute::inout(velBuffer), compute::karg(deltaTime, "dt"), compute::karg(center, "center"));
        /*watch.end();
        log::debug("OpenCL position update elapsed time: {}ms",watch.elapsed_time().milliseconds());*/
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
