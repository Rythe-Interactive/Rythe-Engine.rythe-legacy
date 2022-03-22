#pragma once
#include <core/types/types.hpp>
#include <core/platform/platform.hpp>
#include <core/particles/particlepolicy.hpp>

namespace legion::core
{
    struct particle_emitter;

    struct example_policy : public particle_policy<example_policy>
    {
        NO_DTOR_RULE5_NOEXCEPT(example_policy);
        ~example_policy() = default;

        virtual void onInit(particle_emitter& emitter, size_type start, size_type end) override;
    };

    struct orbital_policy : public particle_policy<orbital_policy>
    {
        NO_DTOR_RULE5_NOEXCEPT(orbital_policy);
        ~orbital_policy() = default;

       double C_MASS = 10.f;
       double P_MASS = 1.f;
       double G_FORCE = 100.0f;

        virtual void setup(particle_emitter& emitter) override;
        virtual void onInit(particle_emitter& emitter, size_type start, size_type end) override;
        virtual void onUpdate(particle_emitter& emitter, float deltaTime, size_type count) override;
    };

    struct fountain_policy : public particle_policy<fountain_policy>
    {
        NO_DTOR_RULE5_NOEXCEPT(fountain_policy);
        ~fountain_policy() = default;

        float initForce;

        virtual void onInit(particle_emitter& emitter, size_type start, size_type end) override;
        virtual void onUpdate(particle_emitter& emitter, float deltaTime, size_type count) override;
    };

    struct scale_lifetime_policy : public particle_policy<scale_lifetime_policy>
    {
        NO_DTOR_RULE5_NOEXCEPT(scale_lifetime_policy);
        ~scale_lifetime_policy() = default;

        virtual void setup(particle_emitter & emitter) override;
        virtual void onInit(particle_emitter & emitter, size_type start, size_type end) override;
        virtual void onUpdate(particle_emitter & emitter, float deltaTime, size_type count) override;
    };
    struct boid_policy : public particle_policy<boid_policy>
    {
        NO_DTOR_RULE5_NOEXCEPT(boid_policy);
        ~boid_policy() = default;

        virtual void setup(particle_emitter& emitter) override;
        virtual void onInit(particle_emitter& emitter, size_type start, size_type end) override;
        virtual void onUpdate(particle_emitter& emitter, float deltaTime, size_type count) override;
    }
}
