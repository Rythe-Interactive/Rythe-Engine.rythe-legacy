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

    template<typename policy>
    struct particle_policy;

    struct life_time
    {
        float age = 0.f;
        float max = 1.f;
        NO_DTOR_RULE5_NOEXCEPT(life_time);
        ~life_time() = default;
    };

    struct example_policy : public particle_policy<example_policy>
    {
        NO_DTOR_RULE5_NOEXCEPT(example_policy);
        ~example_policy() = default;

        virtual void OnInit(particle_emitter& emitter, size_type idx) override;
        virtual void OnUpdate(particle_emitter& emitter, float deltaTime, size_type count) override;
        virtual void OnDestroy(particle_emitter& emitter) override;
    };

    struct orbital_policy : public particle_policy<orbital_policy>
    {
        NO_DTOR_RULE5_NOEXCEPT(orbital_policy);
        ~orbital_policy() = default;

        const double C_MASS = 100.f;
        const double P_MASS = 10.f;
        const double G_FORCE = .1f;

        virtual void OnInit(particle_emitter& emitter, size_type idx) override;
        virtual void OnUpdate(particle_emitter& emitter, float deltaTime, size_type count) override;
        virtual void OnDestroy(particle_emitter& emitter) override;
    };

    class ParticleSystem final : public System<ParticleSystem>
    {
    public:
        void setup();
        void shutdown();
        void update(time::span deltaTime);
        void emit(particle_emitter& emitter);
        void maintanence(particle_emitter& emitter, float deltaTime);
        void printParticles(particle_emitter& emitter);
    };
}
