#pragma once
#include <core/particles/particleemitter.hpp>

namespace legion::core
{
    template<typename ...Policies>
    template<typename buffer>
    L_ALWAYS_INLINE particle_buffer<buffer> particle_emitter<>::getBuffer<buffer>()
    {
        return particleBuffers[typeHash<buffer>()].get();
    }

    template<typename ...Policies>
    L_ALWAYS_INLINE void particle_emitter<>::Emit()
    {
        //add particle id to all buffers
        if (spawnCount < size)
            for (id_type& bufferId : particleBuffers)
                particleBuffers[bufferId]->emplace(spawnCount++, );

        //initialize particle lifetime
        for (auto& [id, lifeTime] : emitter.getBuffer<life_time>())
            lifeTime.max = mineLifeTime + std::rand() * (maxLifeTime - minLifeTime);

        std::apply([](auto&... policy) {((policy->OnInit()), ...); }, policies);
    }

    template<typename ...Policies>
    L_ALWAYS_INLINE void particle_emitter<>::Update(float deltaTime)
    {
        //particle lifetime updates
        for (auto& [id, lifeTime] : emitter.getBuffer<life_time>())
        {
            lifeTime.age += deltaTime;
            if (lifeTime.age >= lifeTime.max)
                for (id_type& bufferId : particleBuffers)
                    particleBuffers[bufferId]->erase(id);
        }

        //particle policy updates
        std::apply([](auto&... policy) {((policy->OnUpdate(deltaTime, spawnCount)), ...); }, policies);
    }
}
