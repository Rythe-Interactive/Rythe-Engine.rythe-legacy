#include <core/particles/particleemitter.hpp>

namespace legion::core
{
    void particle_emitter::play() noexcept
    {
        m_pause = false;
    }

    void particle_emitter::pause() noexcept
    {
        m_pause = true;
    }

    void particle_emitter::stop()
    {
        pause();
        m_particleCount = 0;
    }

    void particle_emitter::set_spawn_rate(size_type rate) noexcept
    {
        m_spawnRate = rate;
    }

    void particle_emitter::set_spawn_interval(float interval) noexcept
    {
        m_spawnInterval = interval;
    }

    void particle_emitter::set_particle_space(Space coordSpace) noexcept
    {
        localSpace = coordSpace == Space::WORLD;
    }

    void particle_emitter::set_alive(size_type idx, bool alive)
    {
        m_livingBuffer.at(idx) = alive;
        if (!alive)
        {
            swap(idx, m_particleCount - 1);
            m_particleCount--;
        }
    }

    void particle_emitter::set_alive(size_type start, size_type count, bool alive)
    {
        std::fill_n(m_livingBuffer.begin() + start, count, alive);
    }

    bool particle_emitter::is_alive(size_type idx) const
    {
        if (idx < m_livingBuffer.size())
            return m_livingBuffer.at(idx);
        return false;
    }

    void particle_emitter::swap(size_type idx1, size_type idx2)
    {
        m_livingBuffer.swap(m_livingBuffer.at(idx1), m_livingBuffer.at(idx2));

        for (auto& [id, buffer] : m_particleBuffers)
        {
            m_particleBuffers[id]->swap(idx1, idx2);
        }
    }

    size_type particle_emitter::size() noexcept
    {
        return m_particleCount;
    }

    void particle_emitter::resize(size_type size)
    {
        m_livingBuffer.resize(size);
        m_capacity = size;
        for (auto& [id, buffer] : m_particleBuffers)
            m_particleBuffers[id]->resize(size);
    }

    size_type particle_emitter::capacity() const noexcept
    {
        return m_capacity;
    }
}
