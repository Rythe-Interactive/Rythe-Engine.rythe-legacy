#pragma once
#include <core/math/math.hpp>
#include <core/defaults/defaultcomponents.hpp>

/**
 * @file particlepolicy.hpp
 * @brief
 */

namespace legion::core
{
    struct particle_emitter;

    template<typename bufferType>
    struct particle_buffer;
    template<typename uniformType>
    struct particle_uniform;

    struct particle_policy_base
    {
        NO_DTOR_RULE5_NOEXCEPT(particle_policy_base);
        virtual ~particle_policy_base() = default;

        virtual void setup(particle_emitter& emitter) LEGION_IMPURE;
        virtual void onInit(particle_emitter& emitter, size_type start, size_type end) LEGION_IMPURE;
        virtual void onUpdate(particle_emitter& emitter, float deltaTime, size_type count) LEGION_IMPURE;
        virtual void onDestroy(particle_emitter& emitter, size_type start, size_type end) LEGION_IMPURE;
    };

    template<typename policy>
    struct particle_policy : public particle_policy_base
    {
        NO_DTOR_RULE5_NOEXCEPT(particle_policy);
        virtual ~particle_policy() = default;
    };
}

#include <core/particles/particlepolicy.inl>
