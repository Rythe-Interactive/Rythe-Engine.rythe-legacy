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
    template<typename policy>
    struct particle_policy;

    struct particle_emitter
    {
    public:
        //Maxium amount of particles allowed in an emitter      default: 1000
        size_type maxSpawnCount = 1000;
        size_type currentParticleCount = 0;
        //How many particles can spawn every interval     default: 1
        size_type spawnRate = 1;
        float spawnBuffer = 0.0f;

        //Minimum life time for a particle      default: 1.f
        float minLifeTime = 1.f;
        //Maxium life time for a particle       default: 2.f
        float maxLifeTime = 2.f;
        //How much time must pass before you spawn more particles       default: .1f
        float spawnInterval = .1f;
        //Elapsed time since last spawn interval        default: 0.f
        float elapsedTime = 0.f;

        //If enabled the particle system will be paused         default: false
        bool pause = false;
        //If enabled no particles can die and the emitter will keep spawning them until it reaches the limit        default: false
        bool infinite = false;
        //If enabled particles positions will be in the local space of the emitter      default: true
        bool localPosition = true;
        //If enabled particles rotations will be in the local space of the emitter      default: true
        bool localRotation = true;
        //If enabled particles scales will be in the local space of the emitter     default: true
        bool localScale = true;

    private:
        std::vector<bool> livingBuffer{};
        std::vector<std::unique_ptr<particle_policy_base>> particlePolicies;
        std::unordered_map<id_type, std::unique_ptr<particle_buffer_base>> particleBuffers;
        std::unordered_map<id_type, std::unique_ptr<particle_uniform_base>> particleUniforms;

    public:
        particle_emitter() = default;
        MOVE_FUNCS(particle_emitter);
        particle_emitter(const particle_emitter&) = delete;
        ~particle_emitter() = default;

        template<typename bufferType,typename... Args>
        particle_buffer<bufferType>& create_buffer(const std::string_view& name, Args&&... args);

        template<typename bufferType>
        particle_buffer<bufferType>& get_buffer(const std::string_view& name);

        template<typename uniformType,typename... Args>
        uniformType& create_uniform(const std::string_view& name, Args&&... args);

        template<typename uniformType>
        uniformType& get_uniform(const std::string_view& name);

        template<typename Policy>
        particle_policy<Policy>& add_policy();
        template<typename Policy>
        particle_policy<Policy>& add_policy(Policy policy);

        void set_alive(size_type idx, bool alive);
        void set_alive(size_type start, size_type end, bool alive);
        bool is_alive(size_type idx);

        void swap(size_type idx1, size_type idx2);
        void resize(size_type size);

        friend class ParticleSystem;
    };
}

#include <core/particles/particleemitter.inl>
