#pragma once
#include <core/types/types.hpp>
#include <core/particles/particle.hpp>
/**
 * @file particleemitter.hpp
 * @brief Particle emitters handle spawning, maintenance, and destruction of particles
 */

namespace legion::core
{
    template<size_type size = 0, typename... Policies>
    struct particle_emitter
    {
        id_type id;
        float spawnRate;
        float spawnBuffer;
        float spawnCount;
        std::unordered_map<id_type, std::unique_ptr<particle_buffer_base>> buffers;
        std::unordered_map<id_type,std::unique_ptr<particle_policy_base>> policies;

        particle_emitter() = default;
        ~particle_emitter() = default;

        template<typename buffer>
        buffer getBuffer()
        {
            return buffers[typeHash<buffer>()].get();
        }
    };
}
