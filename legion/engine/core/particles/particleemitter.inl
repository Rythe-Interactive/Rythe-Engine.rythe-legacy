#include <core/particles/particleemitter.hpp>
#include <core/particles/particlepolicy.hpp>
#include <core/particles/particlebuffer.hpp>
#include <core/particles/particleuniform.hpp>
#pragma once

namespace legion::core
{
    template<typename bufferType>
    particle_buffer<bufferType>& particle_emitter::create_buffer(const std::string_view& name, particle_buffer<bufferType> buffer)
    {
        id_type idName = nameHash(name);
        particleBuffers.try_emplace(idName, std::make_unique<particle_buffer<bufferType>>(buffer));
        return *dynamic_cast<particle_buffer<bufferType>*>(particleBuffers[idName].get());
    }

    template<typename bufferType>
    particle_buffer<bufferType>& particle_emitter::create_buffer(const std::string_view& name, std::vector<bufferType> buffer)
    {
        id_type idName = nameHash(name);
        particleBuffers.try_emplace(idName, std::make_unique<particle_buffer<bufferType>>(buffer));
        return *dynamic_cast<particle_buffer<bufferType>*>(particleBuffers[idName].get());
    }

    template<typename bufferType>
    particle_buffer<bufferType>& particle_emitter::create_buffer(const std::string_view& name)
    {
        id_type idName = nameHash(name);
        particleBuffers.try_emplace(idName, std::make_unique<particle_buffer<bufferType>>());
        return *dynamic_cast<particle_buffer<bufferType>*>(particleBuffers[idName].get());
    }

    template<typename bufferType>
    particle_buffer<bufferType>& particle_emitter::get_buffer(const std::string_view& name)
    {
        id_type idName = nameHash(name);
        if (particleBuffers.count(idName) < 1)
        {
            log::warn("Buffer \"{}\" of type {} does not exist, one will be created for you", name, typeid(bufferType).name());
            create_buffer<bufferType>(name);
        }
        return *dynamic_cast<particle_buffer<bufferType>*>(particleBuffers[idName].get());
    }


    template<typename uniformType>
    uniformType& particle_emitter::create_uniform(const std::string_view& name, particle_uniform<uniformType> val)
    {
        return create_uniform<uniformType>(name, val.uniform);
    }

    template<typename uniformType>
    uniformType& particle_emitter::create_uniform(const std::string_view& name, uniformType val)
    {
        auto& value = create_uniform<uniformType>(name);
        value = val;
        return value;
    }

    template<typename uniformType>
    uniformType& particle_emitter::create_uniform(const std::string_view& name)
    {
        id_type idName = nameHash(name);
        if (particleUniforms.count(idName) < 1)
            particleUniforms.try_emplace(idName, std::make_unique<particle_uniform<uniformType>>());

        return get_uniform<uniformType>(name);
    }

    template<typename uniformType>
    uniformType& particle_emitter::get_uniform(const std::string_view& name)
    {
        id_type idName = nameHash(name);
        if (particleUniforms.count(idName) < 1)
            create_uniform<uniformType>(name);
        auto& uniform = *dynamic_cast<particle_uniform<uniformType>*>(particleUniforms[idName].get());
        return uniform.get();
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