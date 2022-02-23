#include <core/particles/particlesystem.hpp>
#include <core/particles/particleemitter.hpp>
#include <core/common/debugrendering.hpp>

namespace legion::core
{

    void ParticleSystem::setup()
    {
        log::filter(log::severity_debug);
        log::debug("ParticleSystem setup");
    }

    void ParticleSystem::shutdown()
    {
        log::debug("ParticleSystem shutdown");
    }

    void ParticleSystem::update(time::span deltaTime)
    {
        ecs::filter<particle_emitter> filter;
        for (auto& ent : filter)
        {
            auto emitter = ent.get_component<particle_emitter>();
            if (emitter->currentParticleCount < emitter->maxSpawnCount)
                emit(emitter);

            maintanence(emitter, deltaTime);
        }
    }

    void ParticleSystem::emit(particle_emitter& emitter)
    {

        int startIdx = emitter.currentParticleCount;
        int targetIdx = startIdx + emitter.spawnRate;
        for (int i = startIdx; i < targetIdx; i++)
        {
            log::debug("spawned particle: " + std::to_string(i));
            emitter.getBuffer<life_time>().get(i).max = math::linearRand(emitter.minLifeTime, emitter.maxLifeTime);
            emitter.getBuffer<position>().get(i) = math::sphericalRand(1.f);
            auto color = math::sphericalRand(1.f);
            emitter.getBuffer<math::color>().get(i) = math::color(color.x, color.y, color.z, 1.f);
            emitter.getBuffer<velocity>().get(i) = math::sphericalRand(1.f);
        }

        emitter.currentParticleCount += emitter.spawnRate;
        if (emitter.currentParticleCount >= emitter.maxSpawnCount)
            log::debug("Done Spawning");
    }

    void ParticleSystem::maintanence(particle_emitter& emitter, float deltaTime)
    {
        auto& buffer = emitter.getBuffer<life_time>();
        std::vector<id_type> toDelete;
        for (auto& [id, lifeTime] : buffer.buffer)
        {
            lifeTime.age += deltaTime;

            if (lifeTime.age > lifeTime.max)
            {
                log::debug("particle destroyed: " + std::to_string(id));
                toDelete.push_back(id);
            }
        }

        auto& posBuffer = emitter.getBuffer<position>();
        auto& colorBuffer = emitter.getBuffer<math::color>();
        auto& velBuffer = emitter.getBuffer<velocity>();

        for (auto id : toDelete)
        {
            buffer.buffer.erase(id);
            posBuffer.buffer.erase(id);
            colorBuffer.buffer.erase(id);
            velBuffer.buffer.erase(id);
        }

        for (auto& [id, pos] : posBuffer.buffer)
        {
            math::vec3 perp;

            auto& vel = emitter.getBuffer<velocity>().get(id);

            perp = math::normalize(math::cross(vel, math::vec3::up));

            math::vec3 rotated = (math::axisAngleMatrix(vel, math::perlin(pos) * math::pi<float>()) * math::vec4(perp.x, perp.y, perp.z, 0)).xyz();
            rotated.y -= 0.5f;
            rotated = math::normalize(rotated);

            vel = math::normalize(vel + rotated * deltaTime);

            if (math::abs(vel.y) >= 0.9f)
            {
                auto rand = math::circularRand(1.f);
                vel.y = 0.9f;
                vel = math::normalize(vel + math::vec3(rand.x, 0.f, rand.y));
            }

            pos += vel * 0.3f * deltaTime;

            debug::drawLine(math::vec3::zero, pos, colorBuffer.get(id));
        }
    }
}
