#pragma once
#include <rendering/rendering.hpp>

class MouseHover : public lgn::gfx::RenderStage<MouseHover>
{
private:
    gfx::framebuffer m_fbo;
    gfx::texture_handle m_entityIdTexture;

public:
    virtual void setup(lgn::app::window& context) override;
    virtual void render(lgn::app::window& context, lgn::gfx::camera& cam, const lgn::gfx::camera::camera_input& camInput, lgn::time::span deltaTime) override;
    virtual lgn::priority_type priority() override;
};
