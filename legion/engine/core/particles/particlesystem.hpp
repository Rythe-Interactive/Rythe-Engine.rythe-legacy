#pragma once
#include<core/engine/system.hpp>
#include<core/ecs/ecs.hpp>
#include<core/particles/particleemitter.hpp>

/**
 * @file particlesystem.hpp
 * @brief A particle system manages one or more emitters
 */

namespace legion::core
{
    class ParticleSystem : public System<ParticleSystem>
    {
        void Update(float deltaTime)
        {
            ecs::filter<particle_emitter<>> filter;
            for (auto& entity : filter)
            {
                auto& emitter = entity.get_component<particle_emitter<>>().get();
                emitter.spawnBuffer += deltaTime;
                while (emitter.spawnBuffer >= emitter.spawnRate)
                {
                    emitter.spawnBuffer -= emitter.spawnRate;
                }

                for (auto& lifeTime : emitter.getBuffer<int>())
                    lifeTime += deltaTime;

                for (auto& [id, policy] : emitter.policies)
                    policy.get();
            }
        }
    };
}
