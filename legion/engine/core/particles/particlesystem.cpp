#include <core/particles/particles.hpp>
#include <core/common/debugrendering.hpp>


namespace legion::core
{
    void ParticleSystem::setup()
    {
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
            emitter.elapsedTime += deltaTime;

            if (emitter.currentParticleCount < emitter.maxSpawnCount)
            {
                float scaledSpawnRate = (deltaTime / emitter.spawnInterval) * emitter.spawnRate;
                emitter.spawnBuffer += scaledSpawnRate;
                if (emitter.spawnBuffer > emitter.spawnRate)
                {
                    emitter.spawnBuffer = math::min(emitter.spawnBuffer, (float)emitter.maxSpawnCount);
                    emit(emitter, emitter.spawnBuffer);
                    emitter.spawnBuffer -= math::trunc(emitter.spawnBuffer);
                }
            }

            maintenance(emitter, deltaTime);
        }
    }

    void ParticleSystem::emitter_setup(L_MAYBEUNUSED events::component_creation<particle_emitter>& event)
    {
        auto& emitter = event.entity.get_component<particle_emitter>().get();
        for (size_type i = 0; i < emitter.maxSpawnCount; i++)
            emitter.set_alive(i, false);
    }

    void ParticleSystem::emit(particle_emitter& emitter, size_type count)
    {
        auto startCount = emitter.currentParticleCount;
        auto targetCount = emitter.currentParticleCount + count;
        if (targetCount >= emitter.maxSpawnCount)
            targetCount = emitter.maxSpawnCount - startCount;

        emitter.resize(targetCount);
        emitter.set_alive(startCount, targetCount, true);
        emitter.currentParticleCount = targetCount;

        auto& ageBuffer = emitter.get_buffer<life_time>("lifetimeBuffer");
        for (size_type idx = startCount; idx < targetCount; idx++)
        {
            ageBuffer.at(idx).age = 0;
            ageBuffer.at(idx).max = math::linearRand(emitter.minLifeTime, emitter.maxLifeTime);
        }

        for (auto& policy : emitter.particlePolicies)
            policy->OnInit(emitter, startCount, targetCount);
    }


    void ParticleSystem::maintenance(particle_emitter& emitter, float deltaTime)
    {
        auto& ageBuffer = emitter.get_buffer<life_time>("lifetimeBuffer");
        size_type destroyed = 0;
        size_type activeCount = 0;
        for (activeCount = 0; activeCount < emitter.currentParticleCount; activeCount++)
        {
            if (!emitter.is_alive(activeCount))
                break;

            auto& lifeTime = ageBuffer[activeCount];
            lifeTime.age += deltaTime;
        }
        if (!emitter.infinite)
        {
            for (size_type i = 0; i < activeCount; i++)
            {
                auto& lifeTime = ageBuffer[i];
                if (lifeTime.age > lifeTime.max)
                {
                    emitter.set_alive(i, false);
                    emitter.swap(i, emitter.currentParticleCount - 1);
                    emitter.currentParticleCount--;
                    destroyed++;
                }
            }
            auto targetCount = emitter.currentParticleCount + destroyed;
            for (auto& policy : emitter.particlePolicies)
                policy->OnDestroy(emitter, emitter.currentParticleCount, targetCount);

            emitter.resize(emitter.currentParticleCount);
        }

        for (auto& policy : emitter.particlePolicies)
            policy->OnUpdate(emitter, deltaTime, emitter.currentParticleCount);
    }
}
