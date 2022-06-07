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
        id_type nameId = nameHash(name);
        auto [it, _] = m_particleBuffers.emplace(nameId, std::make_unique<particle_buffer<bufferType>>(std::forward<Args>(args)...));
        auto& bffr = *dynamic_cast<particle_buffer<bufferType>*>(it->second.get());
        bffr.resize(m_capacity);
        return bffr;
    }

    template<typename bufferType>
    particle_buffer<bufferType>& particle_emitter::get_buffer(const std::string_view& name)
    {
        id_type nameId = nameHash(name);
        return get_buffer<bufferType>(nameId);
    }

    template<typename bufferType>
    particle_buffer<bufferType>& particle_emitter::get_buffer(id_type nameId)
    {
        return *dynamic_cast<particle_buffer<bufferType>*>(m_particleBuffers.at(nameId).get());
    }

    template<typename uniformType>
    bool particle_emitter::has_buffer(const std::string_view& name)noexcept
    {
        return m_particleBuffers.count(nameHash(name)) > 0;
    }

    template<typename uniformType>
    bool particle_emitter::has_buffer(id_type nameId) noexcept
    {
        return m_particleBuffers.count(nameId) > 0;
    }

    template<typename uniformType, typename... Args>
    uniformType& particle_emitter::create_uniform(const std::string_view& name, Args&&... args)
    {
        id_type nameId = nameHash(name);
        return dynamic_cast<particle_uniform<uniformType>*>(m_particleUniforms.emplace(nameId, std::make_unique<particle_uniform<uniformType>>(std::forward<Args>(args)...)).first->second.get())->get();
    }

    template<typename uniformType>
    uniformType& particle_emitter::get_uniform(const std::string_view& name)
    {
        id_type nameId = nameHash(name);
        return get_uniform<uniformType>(nameId);
    }

    template<typename uniformType>
    uniformType& particle_emitter::get_uniform(id_type nameId)
    {
        return dynamic_cast<particle_uniform<uniformType>*>(m_particleUniforms.at(nameId).get())->get();
    }

    template<typename uniformType>
    bool particle_emitter::has_uniform(const std::string_view& name) noexcept
    {
        return m_particleUniforms.count(nameHash(name)) > 0;
    }

    template<typename uniformType>
    bool particle_emitter::has_uniform(id_type nameId) noexcept
    {
        return m_particleUniforms.count(nameId) > 0;
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
