#pragma once
#include <core/types/types.hpp>
#include <core/platform/platform.hpp>
#include <core/particles/particlepolicy.hpp>
#include <rendering/data/material.hpp>
#include <rendering/data/model.hpp>

namespace legion::rendering
{
    using namespace core;
    struct rendering_policy : core::particle_policy<rendering_policy>
    {
        model_handle model;
        material_handle material;

        NO_DTOR_RULE5_NOEXCEPT(rendering_policy);
        rendering_policy(material_handle _material, model_handle _model) : model(_model), material(_material) {}
        rendering_policy(model_handle _model, material_handle _material) : model(_model), material(_material) {}
        ~rendering_policy() = default;

        virtual void OnSetup(particle_emitter& emitter) override;
        virtual void OnInit(particle_emitter& emitter, size_type start, size_type end) override;
        virtual void OnUpdate(particle_emitter& emitter, float deltaTime, size_type count) override;
        virtual void OnDestroy(particle_emitter& emitter, size_type start, size_type end) override;
    };
}
