#include <core/particles/particleemitter.hpp>

namespace legion::core
{
    void particle_emitter::setAlive(size_type idx, bool alive)
    {
        if (idx < livingBuffer.size())
            livingBuffer[idx] = alive;
        else
        {
            while (idx >= livingBuffer.size())
            {
                livingBuffer.push_back(true);
            }
            livingBuffer[idx] = alive;
        }
    }

    bool particle_emitter::isAlive(size_type idx)
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
