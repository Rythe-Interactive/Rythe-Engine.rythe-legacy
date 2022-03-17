#pragma once
#include <unordered_map>
#include <algorithm>

#include <core/containers/pointer.hpp>
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

    enum Space
    {
        WORLD,
        EMITTER
    };

    struct particle_emitter
    {
    private:
        //Maxium amount of particles allowed in an emitter      default: 1000
        size_type m_capacity = 1000;
        //Current particle count
        size_type m_particleCount = 0;
        //How many particles can spawn every interval     default: 1
        size_type m_spawnRate = 1;

        //If enabled the particle system will be paused         default: false
        bool m_pause = false;
        //How much time must pass before you spawn more particles       default: .1f
        float m_spawnInterval = .1f;
        //Regulates the spawnrate to stay consistent over different timesteps
        float m_spawnBuffer = 0.0f;
        //Elapsed time since last spawn interval        default: 0.f
        float m_elapsedTime = 0.f;
    public:
        //Should particles be in the emitters space or the world space      default: true
        bool localSpace = true;
        //If enabled no particles can die and the emitter will keep spawning them until it reaches the limit        default: false
        bool particleAging = false;

    private:
        std::vector<bool> m_livingBuffer{};
        std::vector<std::unique_ptr<particle_policy_base>> m_particlePolicies;
        std::unordered_map<id_type, std::unique_ptr<particle_buffer_base>> m_particleBuffers;
        std::unordered_map<id_type, std::unique_ptr<particle_uniform_base>> m_particleUniforms;

    public:
        particle_emitter() = default;
        MOVE_FUNCS(particle_emitter);
        particle_emitter(const particle_emitter&) = delete;
        ~particle_emitter() = default;

        template<typename bufferType,typename... Args>
        particle_buffer<bufferType>& create_buffer(const std::string_view& name, Args&&... args);

        template<typename bufferType>
        particle_buffer<bufferType>& get_buffer(const std::string_view& name);
        template<typename bufferType>
        particle_buffer<bufferType>& get_buffer(id_type nameId);

        template<typename uniformType>
        bool has_buffer(const std::string_view& name) noexcept;
        template<typename uniformType>
        bool has_buffer(id_type nameId) noexcept;

        template<typename uniformType,typename... Args>
        uniformType& create_uniform(const std::string_view& name, Args&&... args);

        template<typename uniformType>
        uniformType& get_uniform(const std::string_view& name);
        template<typename uniformType>
        uniformType& get_uniform(id_type nameId);

        template<typename uniformType>
        bool has_uniform(const std::string_view& name) noexcept;
        template<typename uniformType>
        bool has_uniform(id_type nameId) noexcept;

        template<typename Policy, typename... Args>
        particle_policy<Policy>& add_policy(Args&&... args);

        //Plays the particle emitter
        void play() noexcept;
        //Pauses the particle emitter
        void pause() noexcept;
        //Pauses and resets the particle emitter
        void stop();

        //Set how many particles should spawn per intereval
        void set_spawn_rate(size_type rate) noexcept;
        //Set the interval of particle spawning
        void set_spawn_interval(float interval) noexcept;
        //Set what space particles will be in
        void set_particle_space(Space coordSpace) noexcept;
        //Enables a particle
        void set_alive(size_type idx, bool alive);
        //Enables a range of particles
        void set_alive(size_type start, size_type count, bool alive);
        //Returns whether the particle is alive or dead
        bool is_alive(size_type idx) const;

        //Swaps two particles indecies in every buffer
        void swap(size_type idx1, size_type idx2);
        //Returns the current living particle count 
        size_type size() noexcept;
        //Resizes all buffers to the chosen size, and modifies the capacity
        void resize(size_type size);
        //Returns the maximum particle count
        size_type capacity() const noexcept;

        friend class ParticleSystem;
    };
}

#include <core/particles/particleemitter.inl>
