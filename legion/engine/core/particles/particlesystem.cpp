#include <core/particles/particles.hpp>
#include <core/common/debugrendering.hpp>


namespace legion::core
{
    void ParticleSystem::setup()
    {
        log::filter(log::severity_debug);
        log::debug("ParticleSystem setup");

        ecs::filter<particle_emitter> filter;
        for (auto& ent : filter)
        {
            auto emitter = ent.get_component<particle_emitter>();
            for (size_type i = 0; i < emitter->maxSpawnCount; i++)
                emitter->setAlive(i, false);
        }
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
            auto& emitter = ent.get_component<particle_emitter>().get();
            if (emitter.spawn && emitter.currentParticleCount < emitter.maxSpawnCount)
            {
                emitter.elapsedTime += deltaTime;
                if (emitter.elapsedTime >= emitter.spawnInterval)
                {
                    emitter.elapsedTime = 0;
                    emit(emitter);
                }
            }
            maintanence(emitter, deltaTime);
        }
    }

    void ParticleSystem::emit(particle_emitter& emitter)
    {
        auto& ageBuffer = emitter.getBuffer<life_time>();

        size_type targetIdx = emitter.currentParticleCount;

        emitter.setAlive(targetIdx, true);
        ageBuffer.get(targetIdx).age = 0;
        ageBuffer.get(targetIdx).max = math::linearRand(emitter.minLifeTime, emitter.maxLifeTime);
        for (auto& policy : emitter.particlePolicies)
            policy->OnInit(emitter, emitter.currentParticleCount);

        emitter.currentParticleCount++;
        if (emitter.currentParticleCount >= emitter.maxSpawnCount)
        {
            if (!emitter.infinite)
                emitter.spawn = false;
            emitter.currentParticleCount = emitter.maxSpawnCount;
        }
    }

    void ParticleSystem::maintanence(particle_emitter& emitter, float deltaTime)
    {
        auto& ageBuffer = emitter.getBuffer<life_time>();
        for (size_type idx = 0; idx < emitter.currentParticleCount; idx++)
        {
            if (!emitter.isAlive(idx))
                continue;

            auto& lifeTime = ageBuffer[idx];
            lifeTime.age += deltaTime;

            if (lifeTime.age > lifeTime.max)
            {
                emitter.setAlive(idx, false);
                emitter.swap(idx);
                emitter.currentParticleCount--;
                for (auto& policy : emitter.particlePolicies)
                    policy->OnDestroy(emitter);
            }
        }

        for (auto& policy : emitter.particlePolicies)
            policy->OnUpdate(emitter, deltaTime, emitter.currentParticleCount);
    }

    void ParticleSystem::printParticles(particle_emitter& emitter)
    {
        log::debug("Particles");
        for (size_type i = 0; i < emitter.livingBuffer.size(); i++)
        {
            log::debug("\t {} [{}]", i, emitter.livingBuffer[i]);
        }
        log::debug("");
    }

    void example_policy::OnInit(particle_emitter& emitter, size_type idx)
    {
        auto color = math::color(math::linearRand(0.f, 1.f), math::linearRand(0.f, 1.f), math::linearRand(0.f, 1.f), 1);
        emitter.getBuffer<math::color>()[idx] = color;
    }

    void example_policy::OnUpdate(particle_emitter& emitter, float deltaTime, size_type count)
    {
        auto& posBuffer = emitter.getBuffer<position>();
        auto& velBuffer = emitter.getBuffer<velocity>();
        auto& colorBuffer = emitter.getBuffer<math::color>();

        for (size_type idx = 0; idx < count; idx++)
        {
            auto& pos = posBuffer[idx];
            auto& vel = velBuffer[idx];
            debug::drawLine(pos, pos + vel * deltaTime * 30.f, colorBuffer[idx], 5.f);
        }
    }

    void example_policy::OnDestroy(particle_emitter& emitter)
    {
        //log::debug("Particle Destroyed");
    }

    void orbital_policy::OnInit(particle_emitter& emitter, size_type idx)
    {
        emitter.getBuffer<position>()[idx] = math::normalize(math::sphericalRand(1.f)) * 3.f;
        emitter.getBuffer<velocity>()[idx] = math::normalize(math::cross(emitter.getBuffer<position>()[idx], math::vec3::up))*2.f;
    }

    void orbital_policy::OnUpdate(particle_emitter& emitter, float deltaTime, size_type count)
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

    void orbital_policy::OnDestroy(particle_emitter& emitter)
    {

    }
}
