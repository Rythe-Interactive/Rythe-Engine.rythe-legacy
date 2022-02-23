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
        float age = 0.f;
        float max = 1.f;
    };

    struct particle_emitter
    {
        float spawnRate = 2.f;
        float maxSpawnCount = 1000.f;
        float currentParticleCount = 0.f;

        float minLifeTime = 1.f;
        float maxLifeTime = 2.f;

        std::unordered_map<id_type, std::unique_ptr<particle_buffer_base>> particleBuffers;
        //std::unordered_map<id_type, pointer<particle_policy_base>> particlePolicies;

        particle_emitter() = default;
        MOVE_FUNCS(particle_emitter);
        particle_emitter(const particle_emitter&) = delete;
     
        template<typename bufferType>
        particle_buffer<bufferType>& getBuffer();
    };
}

#include <core/particles/particleemitter.inl>
