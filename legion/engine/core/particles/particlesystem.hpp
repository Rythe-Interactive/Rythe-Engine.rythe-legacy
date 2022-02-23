#pragma once
#include <core/engine/system.hpp>
#include <core/ecs/ecs.hpp>
#include <core/time/time.hpp>
/**
 * @file particlesystem.hpp
 * @brief A particle system manages one or more emitters
 */

namespace legion::core
{
    struct particle_emitter;

    class ParticleSystem final : public System<ParticleSystem>
    {
    public:
        void setup();
        void shutdown();
        void update(time::span deltaTime);
        void emit(particle_emitter& emitter);
        void maintanence(particle_emitter& emitter, float deltaTime);
    };
}
