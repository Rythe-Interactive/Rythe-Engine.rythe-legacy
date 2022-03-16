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
            if (emitter.m_pause)
                continue;

            emitter.m_elapsedTime += deltaTime;

            if (emitter.m_particleCount < emitter.m_capacity)
            {
                float scaledSpawnRate = (deltaTime / emitter.m_spawnInterval) * emitter.m_spawnRate;
                emitter.m_spawnBuffer += scaledSpawnRate;
                if (emitter.m_spawnBuffer > emitter.m_spawnRate)
                {
                    emitter.m_spawnBuffer = math::min(emitter.m_spawnBuffer, (float)emitter.m_capacity);
                    emit(emitter, emitter.m_spawnBuffer);
                    emitter.m_spawnBuffer -= math::trunc(emitter.m_spawnBuffer);
                }
            }

            maintenance(emitter, deltaTime);
        }
    }

    void ParticleSystem::emitter_setup(L_MAYBEUNUSED events::component_creation<particle_emitter>& event)
    {
        auto& emitter = event.entity.get_component<particle_emitter>().get();
        emitter.resize(emitter.m_capacity);
    }

    void ParticleSystem::emit(particle_emitter& emitter, size_type count)
    {
        auto startCount = emitter.m_particleCount;
        if (emitter.m_particleCount + count >= emitter.m_capacity)
            count = emitter.m_capacity - startCount;

        emitter.set_alive(startCount, count, true);
        emitter.m_particleCount += count;

        auto& ageBuffer = emitter.get_buffer<life_time>("lifetimeBuffer");
        auto minLifeTime = emitter.has_uniform<float>("minLifeTime") ? emitter.get_uniform<float>("minLifeTime") : 0.f;
        auto maxLifeTime = emitter.has_uniform<float>("maxLifeTime") ? emitter.get_uniform<float>("maxLifeTime") : 0.f;

        for (size_type idx = startCount; idx < emitter.m_particleCount; idx++)
        {
            ageBuffer.at(idx).age = 0;
            ageBuffer.at(idx).max = math::linearRand(minLifeTime, maxLifeTime);
        }

        for (auto& policy : emitter.m_particlePolicies)
            policy->onInit(emitter, startCount, emitter.m_particleCount);
    }


    void ParticleSystem::maintenance(particle_emitter& emitter, float deltaTime)
    {
        auto& ageBuffer = emitter.get_buffer<life_time>("lifetimeBuffer");
        size_type destroyed = 0;
        size_type activeCount = 0;
        for (activeCount = 0; activeCount < emitter.m_particleCount; activeCount++)
        {
            if (!emitter.is_alive(activeCount))
                break;

            auto& lifeTime = ageBuffer[activeCount];
            lifeTime.age += deltaTime;
        }
        if (emitter.has_uniform<float>("minLifeTime") && emitter.has_uniform<float>("maxLifeTime"))
        {
            for (size_type i = 0; i < activeCount; i++)
            {
                auto& lifeTime = ageBuffer[i];
                if (lifeTime.age > lifeTime.max)
                {
                    emitter.set_alive(i, false);
                    emitter.swap(i, emitter.m_particleCount - 1);
                    emitter.m_particleCount--;
                    destroyed++;
                }
            }
            auto targetCount = emitter.m_particleCount + destroyed;
            for (auto& policy : emitter.m_particlePolicies)
                policy->onDestroy(emitter, emitter.m_particleCount, targetCount);
        }

        for (auto& policy : emitter.m_particlePolicies)
            policy->onUpdate(emitter, deltaTime, emitter.m_particleCount);
    }
}
