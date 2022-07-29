#pragma once
#include <rendering/pipeline/base/renderstage.hpp>

namespace legion::rendering
{
    class Skybox : public RenderStage<Skybox>
    {
    public:
        virtual void setup(app::window& context) override;
        virtual void render(app::window& context, camera& cam, const camera::camera_input& camInput, time::span deltaTime) override;
        virtual priority_type priority() override;
    };


}
