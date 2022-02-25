#pragma once
#include <unordered_map>

#include <core/types/types.hpp>

/**
 * @file particleemitter.hpp
 * @brief Particle emitters handle spawning, maintenance, and destruction of particles
 */

namespace legion::core
{
    struct particle_policy_base;
    struct particle_buffer_base;
    template<typename bufferTye>
    struct particle_buffer;

    struct particle_emitter
    {
        size_type maxSpawnCount = 1000;
        float spawnInterval = 1.f;
        float minLifeTime = 1.f;
        float maxLifeTime = 2.f;

        size_type currentParticleCount = 0;
        float elapsedTime = 0.f;
        bool spawn = true;
        bool infinite = false;

        std::vector<bool> livingBuffer{};

        std::unordered_map<id_type, std::unique_ptr<particle_buffer_base>> particleBuffers;
        std::vector<std::unique_ptr<particle_policy_base>> particlePolicies;

        particle_emitter() = default;
        MOVE_FUNCS(particle_emitter);
        particle_emitter(const particle_emitter&) = delete;
        ~particle_emitter() = default;

        template<typename bufferType>
        particle_buffer<bufferType>& getBuffer();

        template<typename... policies>
        void add_policy();

        void swap(size_type idx);
        void setAlive(size_type idx, bool alive);
        bool isAlive(size_type idx);
    };
}

#include <core/particles/particleemitter.inl>
