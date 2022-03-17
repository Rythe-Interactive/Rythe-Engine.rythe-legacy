#pragma once
#include <rendering/pipeline/base/renderstage.hpp>
#include <rendering/pipeline/base/pipeline.hpp>
#include <rendering/components/renderable.hpp>

namespace legion::rendering
{
    class ParticleBatchingStage : public RenderStage<ParticleBatchingStage>
    {
        friend struct particle_emitter;
    public:
        virtual void setup(app::window& context) override;
        virtual void render(app::window& context, camera& cam, const camera::camera_input& camInput, time::span deltaTime) override;
        virtual priority_type priority() override;
    };
}
