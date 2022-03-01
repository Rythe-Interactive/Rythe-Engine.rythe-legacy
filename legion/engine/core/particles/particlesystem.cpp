#include <core/particles/particles.hpp>
#include <core/common/debugrendering.hpp>


namespace legion::core
{
    void ParticleSystem::setup()
    {
        log::filter(log::severity_debug);
        log::debug("ParticleSystem setup");

        bindToEvent<events::component_creation<particle_emitter>, &ParticleSystem::emitter_setup>();
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
                    size_type count = math::trunc(emitter.elapsedTime / emitter.spawnInterval);
                    emit(emitter, count);
                }
            }
            maintanence(emitter, deltaTime);
        }
    }

    void ParticleSystem::emitter_setup(L_MAYBEUNUSED events::component_creation<particle_emitter>& event)
    {
        ecs::filter<particle_emitter> filter;
        for (auto& ent : filter)
        {
            auto emitter = ent.get_component<particle_emitter>();
            for (size_type i = 0; i < emitter->maxSpawnCount; i++)
                emitter->setAlive(i, false);
        }
    }

    void ParticleSystem::emit(particle_emitter& emitter, size_type count)
    {
        auto& ageBuffer = emitter.getBuffer<life_time>();
        if (emitter.currentParticleCount + count > emitter.maxSpawnCount)
        {
            count = emitter.maxSpawnCount - emitter.currentParticleCount;
            if (!emitter.infinite)
                emitter.spawn = false;
        }

        for (size_type i = 0; i < count; i++)
        {
            size_type targetIdx = emitter.currentParticleCount + i;
            emitter.setAlive(targetIdx, true);
            ageBuffer.get(targetIdx).age = 0;
            ageBuffer.get(targetIdx).max = math::linearRand(emitter.minLifeTime, emitter.maxLifeTime);
        }

        for (auto& policy : emitter.particlePolicies)
            policy->OnInit(emitter, emitter.currentParticleCount, emitter.currentParticleCount + count);

        emitter.currentParticleCount += count;
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
                    policy->OnDestroy(emitter, idx, idx);
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
}
