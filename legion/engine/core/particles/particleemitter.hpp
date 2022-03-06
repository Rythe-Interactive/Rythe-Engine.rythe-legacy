#pragma once
#include <unordered_map>

#include <core/platform/platform.hpp>
#include <core/types/types.hpp>

/**
 * @file particleemitter.hpp
 * @brief Particle emitters handle spawning, maintenance, and destruction of particles
 */

namespace legion::core
{
    struct particle_policy_base;
    struct particle_buffer_base;
    struct particle_uniform_base;

    template<typename bufferType>
    struct particle_buffer;
    template<typename uniformType>
    struct particle_uniform;

    struct particle_emitter
    {
        size_type maxSpawnCount = 1000;
        size_type currentParticleCount = 0;
        size_type spawnRate = 1;

        float minLifeTime = 1.f;
        float maxLifeTime = 2.f;
        float spawnInterval = .1f;
        float elapsedTime = 0.f;

        bool spawn = true;
        bool infinite = false;

        std::vector<bool> livingBuffer{};
        std::vector<std::unique_ptr<particle_policy_base>> particlePolicies;
        std::unordered_map<id_type, std::unique_ptr<particle_buffer_base>> particleBuffers;
        std::unordered_map<id_type, std::unique_ptr<particle_uniform_base>> particleUniforms;

        particle_emitter() = default;
        MOVE_FUNCS(particle_emitter);
        particle_emitter(const particle_emitter&) = delete;
        ~particle_emitter() = default;

        template<typename bufferType>
        particle_buffer<bufferType>& create_buffer(const std::string_view& name, particle_buffer<bufferType> buffer);
        template<typename bufferType>
        particle_buffer<bufferType>& create_buffer(const std::string_view& name, std::vector<bufferType> buffer);
        template<typename bufferType>
        particle_buffer<bufferType>& create_buffer(const std::string_view& name);

        template<typename bufferType>
        particle_buffer<bufferType>& get_buffer(const std::string_view& name);


        template<typename uniformType>
        uniformType& create_uniform(const std::string_view& name, particle_uniform<uniformType> val);
        template<typename uniformType>
        uniformType& create_uniform(const std::string_view& name, uniformType val);
        template<typename uniformType>
        uniformType& create_uniform(const std::string_view& name);

        template<typename uniformType>
        uniformType& get_uniform(const std::string_view& name);

        template<typename... policies>
        void add_policy();

        void setAlive(size_type idx, bool alive);
        bool isAlive(size_type idx);
        void swap(size_type idx);
    };
}

#include <core/particles/particleemitter.inl>
