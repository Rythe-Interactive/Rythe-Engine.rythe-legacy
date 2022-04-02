#include "mousehover.hpp"
#include <rendering/pipeline/default/stages/framebufferresizestage.hpp>

using namespace legion;

id_type MouseHover::m_hoveredEntityId = invalid_id;

id_type MouseHover::getHoveredEntityId() noexcept
{
    return m_hoveredEntityId;
}

void MouseHover::setup(app::window& context)
{
    using namespace rendering;

    float renderScale = FramebufferResizeStage::getRenderScale();
    m_framebufferSize = context.framebufferSize();
    m_framebufferSize.x = math::max((int)(m_framebufferSize.x * renderScale), 1);
    m_framebufferSize.y = math::max((int)(m_framebufferSize.y * renderScale), 1);

    app::context_guard guard(context);
    if (!guard.contextIsValid())
        return;

    m_defaultMetaMaterial = MaterialCache::create_material("editor meta", fs::view("assets://shaders/editormeta.shs"));

    m_entityIdTexture = TextureCache::create_texture("entity id texture", m_framebufferSize, {
        texture_type::two_dimensional, false, channel_format::float_hdr, texture_format::rgba_hdr,
        texture_components::rgba, false, true, 0, texture_mipmap::linear, texture_mipmap::linear,
        texture_wrap::repeat, texture_wrap::repeat, texture_wrap::repeat });

    m_depthTexture = TextureCache::create_texture("editor meta depth", m_framebufferSize, {
        texture_type::two_dimensional, false, channel_format::depth_stencil, texture_format::depth_stencil,
        texture_components::depth_stencil, false, false, 0, texture_mipmap::linear, texture_mipmap::linear,
        texture_wrap::repeat, texture_wrap::repeat, texture_wrap::repeat });

    m_fbo = lgn::gfx::framebuffer(GL_FRAMEBUFFER);

    m_fbo.attach(m_entityIdTexture, FRAGMENT_ATTACHMENT);
    m_fbo.attach(m_depthTexture, GL_DEPTH_STENCIL_ATTACHMENT);

    m_fbo.bind();
    uint attachments[1] = { FRAGMENT_ATTACHMENT };
    glDrawBuffers(1, attachments);
    m_fbo.release();
}

void MouseHover::render(app::window& context, gfx::camera& cam, const gfx::camera::camera_input& camInput, time::span deltaTime)
{
    using namespace rendering;
    (void)deltaTime;
    (void)cam;
    static id_type mainId = nameHash("main");
    static id_type batchesId = nameHash("mesh batches");
    static id_type lightsId = nameHash("light buffer");
    static id_type lightCountId = nameHash("light count");
    static id_type matricesId = nameHash("model matrix buffer");
    static id_type entityBufferId = nameHash("entity id buffer");
    static id_type flipbookBufferId = nameHash("flipbook frame buffer");
    static id_type editorMetaVariant = nameHash("editor_meta");

    float renderScale = FramebufferResizeStage::getRenderScale();
    math::ivec2 framebufferSize = context.framebufferSize();
    framebufferSize.x *= renderScale;
    framebufferSize.y *= renderScale;

    if (framebufferSize.x == 0 || framebufferSize.y == 0)
    {
        abort();
        return;
    }

    auto* batches = get_meta<sparse_map<material_handle, sparse_map<model_handle, std::pair<std::vector<id_type>, std::vector<math::mat4>>>>>(batchesId);
    if (!batches)
        return;

    buffer* lightsBuffer = get_meta<buffer>(lightsId);
    if (!lightsBuffer)
        return;

    size_type* lightCount = get_meta<size_type>(lightCountId);
    if (!lightCount)
        return;

    buffer* modelMatrixBuffer = get_meta<buffer>(matricesId);
    if (!modelMatrixBuffer)
        return;

    buffer* entityIdBuffer = get_meta<buffer>(entityBufferId);
    if (!entityIdBuffer)
        return;

    buffer* flipbookBuffer = get_meta<buffer>(flipbookBufferId);
    if (!flipbookBuffer)
        return;

    auto* fbo = getFramebuffer(mainId);
    if (!fbo)
    {
        log::error("Main frame buffer is missing.");
        abort();
        return;
    }

    texture_handle sceneColor;
    auto colorAttachment = fbo->getAttachment(FRAGMENT_ATTACHMENT);
    if (std::holds_alternative<texture_handle>(colorAttachment))
        sceneColor = std::get<texture_handle>(colorAttachment);

    texture_handle sceneNormal;
    auto normalAttachment = fbo->getAttachment(NORMAL_ATTACHMENT);
    if (std::holds_alternative<texture_handle>(normalAttachment))
        sceneNormal = std::get<texture_handle>(normalAttachment);

    texture_handle scenePosition;
    auto positionAttachment = fbo->getAttachment(POSITION_ATTACHMENT);
    if (std::holds_alternative<texture_handle>(positionAttachment))
        scenePosition = std::get<texture_handle>(positionAttachment);

    texture_handle hdrOverdraw;
    auto overdrawAttachment = fbo->getAttachment(OVERDRAW_ATTACHMENT);
    if (std::holds_alternative<texture_handle>(overdrawAttachment))
        hdrOverdraw = std::get<texture_handle>(overdrawAttachment);

    texture_handle sceneDepth;
    auto depthAttachment = fbo->getAttachment(GL_DEPTH_ATTACHMENT);
    if (std::holds_alternative<std::monostate>(depthAttachment))
        depthAttachment = fbo->getAttachment(GL_DEPTH_STENCIL_ATTACHMENT);
    if (std::holds_alternative<texture_handle>(depthAttachment))
        sceneDepth = std::get<texture_handle>(depthAttachment);

    texture_handle skyboxTex;
    if (ecs::world.has_component<skybox_renderer>())
        skyboxTex = ecs::world.get_component<skybox_renderer>()->material.get_param<texture_handle>(SV_SKYBOX);

    app::context_guard guard(context);
    if (!guard.contextIsValid())
    {
        abort();
        return;
    }

    if (framebufferSize != m_framebufferSize)
    {
        m_framebufferSize = framebufferSize;
        m_entityIdTexture.get_texture().resize(framebufferSize);
        m_depthTexture.get_texture().resize(framebufferSize);
    }

    m_fbo.bind();

    glClearColor(0.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto [m, instancesPerMaterial] : *batches)
    {
        material_handle material = m;

        auto currentVariant = material.current_variant();

        if (m.get_name() == "Test")
            material = m_defaultMetaMaterial;
        else
        {
            auto shader = material.get_shader();
            if (!shader.is_valid() || !shader.has_variant(editorMetaVariant))
                material = m_defaultMetaMaterial;
            else
                material.set_variant(editorMetaVariant);
        }

        camInput.bind(material);
        if (material.has_param<uint>(SV_LIGHTCOUNT))
            material.set_param<uint>(SV_LIGHTCOUNT, *lightCount);

        if (sceneColor && material.has_param<texture_handle>(SV_SCENECOLOR))
            material.set_param<texture_handle>(SV_SCENECOLOR, sceneColor);

        if (sceneNormal && material.has_param<texture_handle>(SV_SCENENORMAL))
            material.set_param<texture_handle>(SV_SCENENORMAL, sceneNormal);

        if (scenePosition && material.has_param<texture_handle>(SV_SCENEPOSITION))
            material.set_param<texture_handle>(SV_SCENEPOSITION, scenePosition);

        if (hdrOverdraw && material.has_param<texture_handle>(SV_HDROVERDRAW))
            material.set_param<texture_handle>(SV_HDROVERDRAW, hdrOverdraw);

        if (sceneDepth && material.has_param<texture_handle>(SV_SCENEDEPTH))
            material.set_param<texture_handle>(SV_SCENEDEPTH, sceneDepth);

        if (skyboxTex && material.has_param<texture_handle>(SV_SKYBOX))
            material.set_param(SV_SKYBOX, skyboxTex);

        material.bind();

        for (auto [modelHandle, instances] : instancesPerMaterial)
        {
            if (modelHandle.id == invalid_id)
            {
                for (auto& ent : instances.first)
                    log::warn("Invalid mesh found on entity {}.", ecs::Registry::getEntity(ent)->name);

                continue;
            }

            ModelCache::create_model(modelHandle.id);

            const model& mesh = modelHandle.get_model();

            if (!mesh.buffered)
                modelHandle.buffer_data(*modelMatrixBuffer, *entityIdBuffer, *flipbookBuffer);

            if (mesh.submeshes.empty())
            {
                log::warn("Empty mesh found. Model name: {},  Model ID {}", ModelCache::get_model_name(modelHandle.id), modelHandle.get_mesh().id());
                continue;
            }

            entityIdBuffer->bufferData(instances.first);
            modelMatrixBuffer->bufferData(instances.second);

            {
                mesh.vertexArray.bind();
                mesh.indexBuffer.bind();
                lightsBuffer->bind();

                for (auto submesh : mesh.submeshes)
                    glDrawElementsInstanced(GL_TRIANGLES, (GLuint)submesh.indexCount, GL_UNSIGNED_INT, (GLvoid*)(submesh.indexOffset * sizeof(uint)), (GLsizei)instances.second.size());

                lightsBuffer->release();
                mesh.indexBuffer.release();
                mesh.vertexArray.release();
            }
        }

        material.release();

        if (material != m_defaultMetaMaterial)
            material.set_variant(currentVariant);
    }

    glFlush();
    glFinish();

    auto& texture = m_entityIdTexture.get_texture();
    glBindTexture(static_cast<GLenum>(texture.type), texture.textureId);

    glPixelStoref(GL_UNPACK_ALIGNMENT, 1);
    math::color hoveredColor = math::colors::transparent;

    auto normalizedMousePos = app::InputSystem::getMousePosition();
    normalizedMousePos.y = 1.0 - normalizedMousePos.y;

    auto mousePos = math::ivec2(normalizedMousePos * math::dvec2(m_framebufferSize));
    glReadPixels(mousePos.x, mousePos.y, 1, 1, GL_RGBA, GL_FLOAT, &hoveredColor);

    glBindTexture(static_cast<GLenum>(texture.type), 0);
    m_fbo.release();

    id_type r = static_cast<id_type>(math::iround<float, int64>(hoveredColor.r * 65535.f));
    id_type g = static_cast<id_type>(math::iround<float, int64>(hoveredColor.g * 65535.f));
    id_type b = static_cast<id_type>(math::iround<float, int64>(hoveredColor.b * 65535.f));
    id_type a = static_cast<id_type>(math::iround<float, int64>(hoveredColor.a * 65535.f));
    m_hoveredEntityId = r | (g << 16ull) | (b << 32ull) | (a << 48ull);
}

priority_type MouseHover::priority()
{
    return default_priority;
}
