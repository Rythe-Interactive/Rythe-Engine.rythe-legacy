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

    template<typename... Policies>
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

        particle_emitter(size_type particleCount)  : spawnCount(particleCount)
        {
            //handle policies stuff
        };
        ~particle_emitter() = default;

        template<typename buffer>
        particle_buffer<buffer> getBuffer();
        void Emit();
        void Update(float deltaTime);
    };
}

#include <core/particles/particleemitter.inl>
