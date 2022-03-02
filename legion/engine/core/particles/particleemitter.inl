#include <core/particles/particleemitter.hpp>
#include <core/particles/particlepolicy.hpp>
#include <core/particles/particlebuffer.hpp>
#include <core/particles/particleuniform.hpp>
#pragma once

namespace legion::core
{
    template<typename bufferType>
    void particle_emitter::setBuffer(particle_buffer<bufferType> buffer)
    {
        id_type id = type_hash<bufferType>().value;
        if (particleBuffers.count(id) > 0)
            return;
        particleBuffers.try_emplace(id, std::make_unique<particle_buffer<bufferType>>(buffer));
    }

    template<typename bufferType>
    void particle_emitter::setBuffer(std::vector<bufferType> buffer)
    {
        id_type id = type_hash<bufferType>().value;
        if (particleBuffers.count(id) > 0)
            return;
        particleBuffers.try_emplace(id, std::make_unique<particle_buffer<bufferType>>(buffer));
    }

    template<typename bufferType>
    void particle_emitter::setBuffer()
    {
        id_type id = type_hash<bufferType>().value;
        if (particleBuffers.count(id) > 0)
            return;
        particleBuffers.try_emplace(id, std::make_unique<particle_buffer<bufferType>>());
    }

    template<typename bufferType>
    particle_buffer<bufferType>& particle_emitter::getBuffer()
    {
        id_type id = type_hash<bufferType>().value;
        if (particleBuffers.count(id) < 1)
            setBuffer<bufferType>();
        return *dynamic_cast<particle_buffer<bufferType>*>(particleBuffers[id].get());
    }


    template<typename uniformType>
    void particle_emitter::setUniform(std::string_view name, particle_uniform<uniformType> val)
    {
        if (particleUniforms.count(std::string(name)) < 1)
            particleUniforms.try_emplace(std::string(name), std::make_unique<particle_uniform<uniformType>>());

        auto& unfrm = *dynamic_cast<particle_uniform<uniformType>*>(particleUniforms[std::string(name)].get());
        unfrm.get() = val.uniform;
    }

    template<typename uniformType>
    void particle_emitter::setUniform(std::string_view name, uniformType val)
    {
        if (particleUniforms.count(std::string(name)) < 1)
            particleUniforms.try_emplace(std::string(name), std::make_unique<particle_uniform<uniformType>>());

        auto& unfrm = *dynamic_cast<particle_uniform<uniformType>*>(particleUniforms[std::string(name)].get());
        unfrm.get() = val;
    }

    template<typename uniformType>
    void particle_emitter::setUniform(std::string_view name)
    {
        if (particleUniforms.count(std::string(name)) < 1)
            particleUniforms.try_emplace(std::string(name), std::make_unique<particle_uniform<uniformType>>());
    }

    template<typename uniformType>
    particle_uniform<uniformType>& particle_emitter::getUniform(const std::string_view& name)
    {
        if (particleUniforms.count(std::string(name)) < 1)
        {
            setUniform<uniformType>(std::string(name));
        }
        return *dynamic_cast<particle_uniform<uniformType>*>(particleUniforms[std::string(name)].get());
    }

    template<typename... policies>
    void particle_emitter::add_policy()
    {
        size_type start = particlePolicies.size();
        particlePolicies.push_back((std::make_unique<policies>(), ...));
        size_type end = particlePolicies.size();
        for (size_type idx = start; idx < end; idx++)
        {
            particlePolicies[idx]->OnSetup(*this);
        }
    }
}
