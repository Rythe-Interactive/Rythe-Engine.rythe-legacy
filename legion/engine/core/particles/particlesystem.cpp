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
        OPTICK_EVENT("[Particle System] Update");
        for (auto& ent : filter)
        {
            auto& emitter = ent.get_component<particle_emitter>().get();
            if (emitter.m_pause)
                continue;

            emitter.m_elapsedTime += deltaTime;

            if (emitter.m_particleCount < emitter.m_capacity)
            {
                emitter.m_spawnBuffer += deltaTime;
                if (emitter.m_spawnBuffer > emitter.m_spawnInterval)
                {
                    size_type spawnIterations = static_cast<size_type>(emitter.m_spawnBuffer / emitter.m_spawnInterval);
                    emit(emitter, spawnIterations * emitter.m_spawnRate);
                    emitter.m_spawnBuffer -= emitter.m_spawnInterval * spawnIterations;
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
        OPTICK_EVENT("[Particle System] Emit Step");
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
        OPTICK_EVENT("[Particle System] Maintenance");
        auto& ageBuffer = emitter.get_buffer<life_time>("lifetimeBuffer");
        size_type destroyed = 0;
        size_type activeCount = 0;
        for (activeCount = 0; activeCount < emitter.m_particleCount; activeCount++)
        {
            if (!emitter.is_alive(activeCount))
                break;

            ageBuffer[activeCount].age += deltaTime;
        }
        if (emitter.has_uniform<float>("minLifeTime") && emitter.has_uniform<float>("maxLifeTime"))
        {
            OPTICK_EVENT("[Particle System] Manage Ages");
            for (size_type i = 0; i < activeCount; i++)
            {
                auto& lifeTime = ageBuffer[i];
                if (lifeTime.age > lifeTime.max)
                {
                    OPTICK_EVENT("[Particle System] Destroy Particle");
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
