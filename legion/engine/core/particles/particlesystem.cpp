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
            if (emitter.currentParticleCount < emitter.maxSpawnCount)
            {
                emitter.elapsedTime += deltaTime;
                if (emitter.elapsedTime >= emitter.spawnInterval)
                {
                    emit(emitter, emitter.spawnRate);
                    emitter.elapsedTime = 0;
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
        auto& ageBuffer = emitter.get_buffer<life_time>("lifetimeBuffer");
        if (emitter.currentParticleCount + count > emitter.maxSpawnCount)
            count = emitter.maxSpawnCount - emitter.currentParticleCount;

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
        if (!emitter.infinite)
        {
            auto& ageBuffer = emitter.get_buffer<life_time>("lifetimeBuffer");
            size_type destroyed = 0;
            size_type activeCount = 0;
            for (activeCount = 0; activeCount < emitter.currentParticleCount; activeCount++)
            {
                if (!emitter.isAlive(activeCount))
                    break;

                auto& lifeTime = ageBuffer[activeCount];
                lifeTime.age += deltaTime;
            }

            for (size_type i = 0; i < activeCount; i++)
            {
                auto& lifeTime = ageBuffer[i];
                if (lifeTime.age > lifeTime.max)
                {
                    emitter.setAlive(i, false);
                    emitter.swap(i, emitter.currentParticleCount - 1);
                    emitter.currentParticleCount--;
                    destroyed++;
                }
            }
            for (auto& policy : emitter.particlePolicies)
                policy->OnDestroy(emitter, emitter.currentParticleCount, emitter.currentParticleCount + destroyed);

            emitter.resize(emitter.currentParticleCount);
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
