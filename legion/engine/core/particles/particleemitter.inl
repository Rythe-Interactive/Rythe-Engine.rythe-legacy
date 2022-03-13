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
        return create_buffer<bufferType>(name, buffer.data());
    }

    template<typename bufferType>
    particle_buffer<bufferType>& particle_emitter::create_buffer(const std::string_view& name, std::vector<bufferType> buffer)
    {
        auto& bffr = create_buffer<bufferType>(name);
        bffr.insert(bffr.begin(), buffer.begin(), buffer.end());
        return bffr;
    }

    template<typename bufferType>
    particle_buffer<bufferType>& particle_emitter::create_buffer(const std::string_view& name)
    {
        id_type idName = nameHash(name);
        if (particleBuffers.count(idName) > 0)
        {
            log::warn("Buffer \"{}\" of type {} already exists", name, typeid(bufferType).name());
            return *dynamic_cast<particle_buffer<bufferType>*>(particleBuffers.at(idName).get());
        }
        particleBuffers.try_emplace(idName, std::make_unique<particle_buffer<bufferType>>());
        auto& bffr = *dynamic_cast<particle_buffer<bufferType>*>(particleBuffers.at(idName).get());
        bffr.resize(currentParticleCount);
        return bffr;
    }

    template<typename bufferType>
    particle_buffer<bufferType>& particle_emitter::get_buffer(const std::string_view& name)
    {
        id_type idName = nameHash(name);
        if (particleBuffers.count(idName) < 1)
        {
            log::warn("Buffer \"{}\" of type {} does not exist, one will be created for you", name, typeid(bufferType).name());
            return create_buffer<bufferType>(name);
        }
        return *dynamic_cast<particle_buffer<bufferType>*>(particleBuffers.at(idName).get());
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

    template<typename uniformType,typename... Args>
    uniformType& particle_emitter::create_uniform(const std::string_view& name, Args&&... args)
    {
        id_type idName = nameHash(name);
        if (particleUniforms.count(idName) < 1)
            particleUniforms.try_emplace(idName, std::make_unique<particle_uniform<uniformType>>(std::forward<Args>(args)...));

        return get_uniform<uniformType>(name);
    }

    template<typename uniformType>
    uniformType& particle_emitter::get_uniform(const std::string_view& name)
    {
        id_type idName = nameHash(name);
        if (particleUniforms.count(idName) < 1)
            create_uniform<uniformType>(name);
        auto& uniform = *dynamic_cast<particle_uniform<uniformType>*>(particleUniforms.at(idName).get());
        return uniform.get();
    }

    template<typename Policy>
    particle_policy<Policy>& particle_emitter::add_policy()
    {
        particlePolicies.push_back(std::make_unique<Policy>());
        particlePolicies[particlePolicies.size() - 1]->setup(*this);
        return *dynamic_cast<particle_policy<Policy>*>(particlePolicies.at(particlePolicies.size() - 1).get());
    }

    template<typename Policy>
    particle_policy<Policy>& particle_emitter::add_policy(Policy policy)
    {
        particlePolicies.push_back(std::make_unique<Policy>(policy));
        particlePolicies[particlePolicies.size() - 1]->setup(*this);
        return *dynamic_cast<particle_policy<Policy>*>(particlePolicies.at(particlePolicies.size() - 1).get());
    }
}
