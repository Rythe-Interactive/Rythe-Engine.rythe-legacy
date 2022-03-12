#include <core/particles/particleemitter.hpp>

namespace legion::core
{
    void particle_emitter::set_alive(size_type idx, bool alive)
    {
        if (livingBuffer.size() < 1)
        {
            livingBuffer.push_back(alive);
            return;
        }

        assert_msg("index is greater than maximum particle count", idx <= maxSpawnCount);
        if (idx > livingBuffer.size()-1)
            livingBuffer.insert(livingBuffer.end(), idx - (livingBuffer.size()-1), true);

        livingBuffer[idx] = alive;
    }

    void particle_emitter::set_alive(size_type start, size_type end, bool alive)
    {
        livingBuffer.insert(livingBuffer.begin()+start, end-start, alive);
    }

    bool particle_emitter::is_alive(size_type idx)
    {
        if (idx < livingBuffer.size())
            return livingBuffer[idx];
        return false;
    }

    void particle_emitter::swap(size_type idx1, size_type idx2)
    {
        std::iter_swap(livingBuffer.begin() + idx1, livingBuffer.begin() + idx2);

        for (auto& [id, buffer] : particleBuffers)
            particleBuffers[id]->swap(idx1, idx2);
    }

    void particle_emitter::resize(size_type size)
    {
        for (auto& [id, buffer] : particleBuffers)
            particleBuffers[id]->resize(size);
    }
}
