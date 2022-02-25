#include <core/particles/particleemitter.hpp>
#include <core/particles/particlepolicy.hpp>
#include <core/particles/particlebuffer.hpp>
#pragma once

namespace legion::core
{
    template<typename bufferType>
    particle_buffer<bufferType>& particle_emitter::getBuffer()
    {
        id_type id = type_hash<bufferType>().value;
        auto [iterator, emplaced] = particleBuffers.try_emplace(id, std::make_unique<particle_buffer<bufferType>>());
        return *dynamic_cast<particle_buffer<bufferType>*>(iterator->second.get());
    }

    template<typename... policies>
    void particle_emitter::add_policy()
    {
        particlePolicies.push_back((std::make_unique<policies>(), ...));
    }
}
