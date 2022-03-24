#pragma once
#include <rendering/rendering.hpp>

class MouseHover : public lgn::gfx::RenderStage<MouseHover>
{
private:
    lgn::math::ivec2 m_framebufferSize;
    lgn::gfx::framebuffer m_fbo;
    lgn::gfx::texture_handle m_entityIdTexture;
    lgn::gfx::texture_handle m_depthTexture;
    lgn::gfx::material_handle m_defaultMetaMaterial;

    static lgn::id_type m_hoveredEntityId;

public:
    static lgn::id_type getHoveredEntityId() noexcept;

    virtual void setup(lgn::app::window& context) override;
    virtual void render(lgn::app::window& context, lgn::gfx::camera& cam, const lgn::gfx::camera::camera_input& camInput, lgn::time::span deltaTime) override;
    virtual lgn::priority_type priority() override;
};
