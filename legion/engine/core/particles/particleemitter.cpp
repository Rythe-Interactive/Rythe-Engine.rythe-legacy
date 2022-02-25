#include <core/particles/particleemitter.hpp>

namespace legion::core
{
    void particle_emitter::swap(size_type idx)
    {
        std::iter_swap(livingBuffer.begin() + idx, livingBuffer.begin() + currentParticleCount - 1);

        for (auto& [id, buffer] : particleBuffers)
            particleBuffers[id]->swap(idx, currentParticleCount - 1);
    }

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
        return livingBuffer[idx];
    }
}
