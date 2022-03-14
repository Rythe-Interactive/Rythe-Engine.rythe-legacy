#include <core/particles/particleemitter.hpp>
#include <core/particles/particlepolicy.hpp>
#include <core/particles/particlebuffer.hpp>
#include <core/particles/particleuniform.hpp>
#pragma once

namespace legion::core
{

    template<typename bufferType, typename... Args>
    particle_buffer<bufferType>& particle_emitter::create_buffer(const std::string_view& name, Args&&... args)
    {
        id_type idName = nameHash(name);
        if (m_particleBuffers.count(idName))
        {
            log::warn("Buffer \"{}\" of type {} already exists", name, nameOfType<bufferType>());
            return get_buffer<bufferType>(name);
        }
        auto [it, _] = m_particleBuffers.emplace(idName, std::make_unique<particle_buffer<bufferType>>(std::forward<Args>(args)...));
        auto& bffr = *dynamic_cast<particle_buffer<bufferType>*>(it->second.get());
        bffr.resize(currentParticleCount);
        return bffr;
    }

    template<typename bufferType>
    particle_buffer<bufferType>& particle_emitter::get_buffer(const std::string_view& name)
    {
        id_type idName = nameHash(name);
        if (!m_particleBuffers.count(idName))
        {
            log::warn("Buffer \"{}\" of type {} does not exist, one will be created for you", name, nameOfType<bufferType>());
            return create_buffer<bufferType>(name);
        }
        return *dynamic_cast<particle_buffer<bufferType>*>(m_particleBuffers.at(idName).get());
    }

    template<typename uniformType, typename... Args>
    uniformType& particle_emitter::create_uniform(const std::string_view& name, Args&&... args)
    {
        id_type idName = nameHash(name);
        if (m_particleUniforms.count(idName))
        {
            log::warn("Uniform \"{}\" of type {} already exists", name, nameOfType<uniformType>());
            return get_uniform<uniformType>(name);
        }
        else
            return dynamic_cast<particle_uniform<uniformType>*>(m_particleUniforms.emplace(idName, std::make_unique<particle_uniform<uniformType>>(std::forward<Args>(args)...)).first->second.get())->get();
    }

    template<typename uniformType>
    uniformType& particle_emitter::get_uniform(const std::string_view& name)
    {
        id_type idName = nameHash(name);
        if (!m_particleUniforms.count(idName))
        {
            log::warn("Uniform \"{}\" of type {} does not exist, one will be created for you", name, nameOfType<uniformType>());
            return create_uniform<uniformType>(name);
        }
        return dynamic_cast<particle_uniform<uniformType>*>(m_particleUniforms.at(idName).get())->get();
    }

    template<typename Policy, typename... Args>
    particle_policy<Policy>& particle_emitter::add_policy(Args&&... args)
    {
        m_particlePolicies.push_back(std::make_unique<Policy>(std::forward<Args...>(args)...));
        auto& policy = m_particlePolicies[m_particlePolicies.size() - 1];
        resize(m_capacity);
        policy->setup(*this);
        return *dynamic_cast<particle_policy<Policy>*>(policy.get());
    }
}
