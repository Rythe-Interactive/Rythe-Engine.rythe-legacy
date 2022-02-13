#pragma once
#include <core/types/types.hpp>
#include <core/particles/particlebuffer.hpp>
#include <core/particles/particlepolicy.hpp>
/**
 * @file particleemitter.hpp
 * @brief Particle emitters handle spawning, maintenance, and destruction of particles
 */

namespace legion::core
{
    struct life_time
    {
        float age;
        float max;
    };

    template<size_type size = 0, typename... Policies>
    struct particle_emitter
    {
        id_type id;
        float spawnRate;
        float spawnBuffer;
        float spawnCount;
        float minLifeTime = 1;
        float maxLifeTime = 2;
        std::unordered_map<id_type, std::unique_ptr<particle_buffer_base>> particleBuffers;
        std::unordered_map<id_type, std::unique_ptr<particle_policy_base>> particlePolicies;
        std::tuple<std::unique_ptr<particle_policy_base>> policies;

        particle_emitter()
        {
            polices = std::make_tuple(std::make_unique(...Policies));
        }
        ~particle_emitter() = default;

        template<typename buffer>
        particle_buffer<buffer> getBuffer()
        {
            return particleBuffers[typeHash<buffer>()].get();
        }

        void Emit()
        {
            //add particle id to all buffers
            if (spawnCount < size)
                for (id_type& bufferId : particleBuffers)
                    particleBuffers[bufferId]->emplace(spawnCount++, );

            //initialize particle lifetime
            for (auto& [id, lifeTime] : emitter.getBuffer<life_time>())
                lifeTime.max = mineLifeTime + std::rand() * (maxLifeTime-minLifeTime);

            std::apply([](auto&... policy) {((policy->OnInit()), ...); }, policies);
        }

        void Update(float deltaTime)
        {
            //particle lifetime updates
            for (auto& [id, lifeTime] : emitter.getBuffer<life_time>())
            {
                lifeTime.age += deltaTime;
                if (lifeTime.age >= lifeTime.max)
                    for (id_type& bufferId : particleBuffers)
                        particleBuffers[bufferId]->erase(id);
            }

            //particle policy updates
            std::apply([](auto&... policy) {((policy->OnUpdate(deltaTime,spawnCount)), ...); },policies);
        }
    };
}
