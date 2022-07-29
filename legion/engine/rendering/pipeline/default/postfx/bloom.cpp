#include <rendering/pipeline/default/postfx/bloom.hpp>
#include <rendering/systems/renderer.hpp>

namespace legion::rendering
{
    size_type Bloom::m_blurIterations = 5;

    void Bloom::setIterationCount(size_type count)
    {
        m_blurIterations = math::min(count, static_cast<size_type>(10));
    }

    void Bloom::setup(app::window& context)
    {
        using namespace legion::core::fs::literals;
        // Create all the shaders needed.
        m_brightnessThresholdShader = rendering::ShaderCache::create_shader("bloom brightness threshold", "engine://shaders/bloombrightnessthreshold.shs"_view);
        m_combineShader = rendering::ShaderCache::create_shader("bloom combine", "engine://shaders/bloomcombine.shs"_view);

        m_resampleShader = ShaderCache::create_shader("bloom resample", "engine://shaders/resample.shs"_view);
        auto fbSize = context.size() / 2;
        for (size_type i = 0; i < 10; i++)
        {
            m_downSampleTex[i] = TextureCache::create_texture("bloomTex" + std::to_string(i), fbSize, settings);

            if (fbSize.x == 1 && fbSize.y == 1)
                break;

            fbSize = math::max(fbSize / 2, math::ivec2(1, 1));
        }

        // Creating 2 framebuffers to pingpong texturs with. (used for blurring)
        for (int i = 0; i < 2; i++)
        {
            m_pingpongFrameBuffers[i] = framebuffer(GL_FRAMEBUFFER);
        }
        // Adding itself to the post processing renderpass.
        addRenderPass<&Bloom::renderPass>();
    }

    void Bloom::seperateOverdraw(framebuffer& fbo, texture_handle colortexture, texture_handle depthtexture, texture_handle overdrawtexture)
    {
        // Brightness threshold stage
        fbo.bind();
        // Create 2 color attachments and add them the buffer.
        uint attachments[1] = { OVERDRAW_ATTACHMENT };
        glDrawBuffers(1, attachments);

        // Bind and assign the brightness threshold shader.
        m_brightnessThresholdShader.bind();
        m_brightnessThresholdShader.get_uniform_with_location<texture_handle>(SV_SCENECOLOR).set_value(colortexture);
        m_brightnessThresholdShader.get_uniform_with_location<texture_handle>(SV_SCENEDEPTH).set_value(depthtexture);
        m_brightnessThresholdShader.get_uniform_with_location<texture_handle>(SV_HDROVERDRAW).set_value(overdrawtexture);
        // Render onto the quad.
        renderQuad();
        // Release the shader.
        m_brightnessThresholdShader.release();;

        // detach the second color attachment from the framebuffer.
        fbo.release();
    }

    texture_handle Bloom::blurOverdraw(const math::ivec2& framebufferSize, texture_handle overdrawtexture)
    {
        if (m_downSampleTex[0].get_texture().size() != framebufferSize / 2)
        {
            auto fbSize = framebufferSize;
            for (size_type i = 0; i < 10; i++)
            {
                m_downSampleTex[i].get_texture().resize(fbSize);

                if (fbSize.x == 1 && fbSize.y == 1)
                    break;

                fbSize = math::max(fbSize / 2, math::ivec2(1, 1));
            }
        }

        texture_handle src = overdrawtexture;

        auto sampleProcess = [&](size_type idx, bool upSample)
        {
            auto& fbo = m_pingpongFrameBuffers[idx % 2];
            fbo.attach(m_downSampleTex[idx], FRAGMENT_ATTACHMENT);
            fbo.bind();
            m_resampleShader.bind();
            m_resampleShader.get_uniform_with_location<texture_handle>(SV_SCENECOLOR).set_value(src);
            m_resampleShader.get_uniform<math::vec2>("scale").set_value(math::vec2(math::pow(2.f, static_cast<float>(idx))));
            if (upSample)
                m_resampleShader.get_uniform<texture_handle>("mixTex").set_value(m_downSampleTex[idx]);

            renderQuad();
            m_resampleShader.release();
            fbo.release();
            src = m_downSampleTex[idx];
        };

        m_resampleShader.configure_variant("downsample");

        for (size_type i = 0; i < m_blurIterations; i++)
            sampleProcess(i, false);

        m_resampleShader.configure_variant("upsample");

        for (size_type i = 1; i < m_blurIterations; i++)
            sampleProcess(m_blurIterations - i - 1, true);

        return src;
    }

    void Bloom::combineImages(framebuffer& fbo, texture_handle colortexture, texture_handle overdrawtexture)
    {
        // Combining phase.

        fbo.bind();
        uint attachments[1] = { FRAGMENT_ATTACHMENT };
        glDrawBuffers(1, attachments);

        // bind the combining shader.
        m_combineShader.bind();

        // Assign the lighting data texture and blurred brightness threshold texture.
        m_combineShader.get_uniform_with_location<texture_handle>(SV_SCENECOLOR).set_value(colortexture);
        m_combineShader.get_uniform_with_location<texture_handle>(SV_HDROVERDRAW).set_value(overdrawtexture);
        // Render it onto the quad.
        renderQuad();
        // Release both the combining shader and framebuffer.
        m_combineShader.release();

        uint defaultAttachments[4] = { FRAGMENT_ATTACHMENT, NORMAL_ATTACHMENT, POSITION_ATTACHMENT, OVERDRAW_ATTACHMENT };
        glDrawBuffers(4, defaultAttachments);
        fbo.release();
    }

    void Bloom::renderPass(framebuffer& fbo, RenderPipelineBase* pipeline, camera& cam, const camera::camera_input& camInput, time::span deltaTime)
    {
        // If a brightness threshold texture had not been created yet, create one.
        texture_handle overdrawTexture;

        //Try to get color attachment.
        auto color_attachment = fbo.getAttachment(FRAGMENT_ATTACHMENT);
        if (!std::holds_alternative<texture_handle>(color_attachment)) return;

        //Get color texture.
        auto color_texture = std::get<texture_handle>(color_attachment);

        {
            auto attachment = fbo.getAttachment(OVERDRAW_ATTACHMENT);
            if (std::holds_alternative<texture_handle>(attachment))
                overdrawTexture = std::get<texture_handle>(attachment);
        }


        texture_handle sceneDepth;
        auto depthAttachment = fbo.getAttachment(GL_DEPTH_ATTACHMENT);
        if (std::holds_alternative<std::monostate>(depthAttachment))
            depthAttachment = fbo.getAttachment(GL_DEPTH_STENCIL_ATTACHMENT);
        if (std::holds_alternative<texture_handle>(depthAttachment))
            sceneDepth = std::get<texture_handle>(depthAttachment);

        // Get brightest parts of the scene and append to overdraw buffer.
        seperateOverdraw(fbo, color_texture, sceneDepth, overdrawTexture);

        // Gets the size of the lighting data texture.
        math::ivec2 framebufferSize = color_texture.get_texture().size();

        // Blur the overdraw buffer.
        texture_handle blurredImage = blurOverdraw(framebufferSize, overdrawTexture);

        // Recombine the overdraw texture with the scene color.
        combineImages(fbo, color_texture, blurredImage);
    }

}
