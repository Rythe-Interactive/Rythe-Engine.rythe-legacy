#pragma once
#include <core/engine/system.hpp>
#include <core/ecs/ecs.hpp>
#include <core/time/time.hpp>
#include <core/events/events.hpp>
#include <core/engine/system.inl>

/**
 * @file particlesystem.hpp
 * @brief A particle system manages one or more emitters
 */

namespace legion::core
{
    struct particle_emitter;

    template<typename policy>
    struct particle_policy;

    struct life_time
    {
        float age = 0.f;
        float max = 1.f;
    };

    class ParticleSystem final : public System<ParticleSystem>
    {
    public:
        void setup();
        void shutdown();
        void update(time::span deltaTime);
        void emitter_setup(L_MAYBEUNUSED events::component_creation<particle_emitter>& event);
        void emit(particle_emitter& emitter, size_type count);
        void maintenance(particle_emitter& emitter, float deltaTime);
    };
}
