#pragma once
#include <rendering/pipeline/base/renderstage.hpp>
#include <rendering/pipeline/base/pipeline.hpp>
#include <rendering/data/screen_quad.hpp>

namespace legion::rendering
{
    class TransparencyRenderStage : public RenderStage<TransparencyRenderStage>
    {
    private:
        material_handle m_defaultDepthOnlyMaterial;

    public:
        virtual void setup(app::window& context) override;
        virtual void render(app::window& context, camera& cam, const camera::camera_input& camInput, time::span deltaTime) override;
        virtual priority_type priority() override;
    };
}
