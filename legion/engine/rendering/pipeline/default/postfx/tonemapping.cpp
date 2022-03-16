#include <rendering/pipeline/default/postfx/tonemapping.hpp>

namespace legion::rendering
{
    std::atomic<id_type> Tonemapping::m_currentShader = { nameHash("aces tonemapping") };

    void Tonemapping::setAlgorithm(tonemapping_type type)
    {
        OPTICK_EVENT();
        static id_type acesId = nameHash("aces tonemapping");
        static id_type reinhardId = nameHash("reinhard tonemapping");
        static id_type reinhardJodieId = nameHash("reinhard jodie tonemapping");
        static id_type legionId = nameHash("legion tonemapping");
        static id_type unreal3Id = nameHash("unreal3 tonemapping");

        switch (type)
        {
        case tonemapping_type::aces:
            m_currentShader.store(acesId, std::memory_order_relaxed);
            break;
        case tonemapping_type::reinhard:
            m_currentShader.store(reinhardId, std::memory_order_relaxed);
            break;
        case tonemapping_type::reinhard_jodie:
            m_currentShader.store(reinhardJodieId, std::memory_order_relaxed);
            break;
        case tonemapping_type::legion:
            m_currentShader.store(legionId, std::memory_order_relaxed);
            break;
        case tonemapping_type::unreal3:
            m_currentShader.store(unreal3Id, std::memory_order_relaxed);
            break;
        default:
            m_currentShader.store(legionId, std::memory_order_relaxed);
            break;
        }
    }

    void Tonemapping::setup(app::window& context)
    {
        OPTICK_EVENT();
        using namespace legion::core::fs::literals;
        rendering::ShaderCache::create_shader("aces tonemapping", "engine://shaders/aces.shs"_view);
        rendering::ShaderCache::create_shader("reinhard tonemapping", "engine://shaders/reinhard.shs"_view);
        rendering::ShaderCache::create_shader("reinhard jodie tonemapping", "engine://shaders/reinhardjodie.shs"_view);
        rendering::ShaderCache::create_shader("legion tonemapping", "engine://shaders/legiontonemap.shs"_view);
        rendering::ShaderCache::create_shader("unreal3 tonemapping", "engine://shaders/unreal3.shs"_view);
        addRenderPass<&Tonemapping::renderPass>();
        exposure = 0.75f;
    }

    void Tonemapping::renderPass(framebuffer& fbo, RenderPipelineBase* pipeline, camera& cam, const camera::camera_input& camInput, time::span deltaTime)
    {
        //Try to get color attachment.
        auto color_attachment = fbo.getAttachment(FRAGMENT_ATTACHMENT);
        if (!std::holds_alternative<texture_handle>(color_attachment)) return;

        //Get color texture.
        auto color_texture = std::get<texture_handle>(color_attachment);

        OPTICK_EVENT();
        static id_type exposureId = nameHash("exposure");
        //static bool firstFrame = true;

        auto shader = ShaderCache::get_handle(m_currentShader.load(std::memory_order_relaxed));

        fbo.bind();
        shader.bind();

        shader.get_uniform<float>(exposureId).set_value(exposure);

        shader.get_uniform_with_location<texture_handle>(SV_SCENECOLOR).set_value(color_texture);
        renderQuad();
        shader.release();
        fbo.release();
    }
}
