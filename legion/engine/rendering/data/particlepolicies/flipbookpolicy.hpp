#pragma once
#include <core/types/types.hpp>
#include <core/platform/platform.hpp>
#include <core/particles/particlepolicy.hpp>
#include <rendering/data/material.hpp>
#include <rendering/data/model.hpp>

namespace legion::rendering
{
    using namespace core;
    struct flipbook_policy : core::particle_policy<flipbook_policy>
    {
        NO_DTOR_RULE5_NOEXCEPT(flipbook_policy);
        ~flipbook_policy() = default;

        virtual void setup(particle_emitter& emitter) override;
        virtual void onUpdate(particle_emitter& emitter, float deltaTime, size_type count) override;
    };
}
