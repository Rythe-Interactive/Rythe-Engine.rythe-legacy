#include <rendering/pipeline/default/stages/skybox.hpp>
#include <rendering/components/renderable.hpp>
#include <rendering/util/bindings.hpp>

namespace legion::rendering
{
    void Skybox::setup(app::window& context)
    {
    }

    void Skybox::render(app::window& context, camera& cam, const camera::camera_input& camInput, time::span deltaTime)
    {
        ecs::filter<skybox_renderer> filter;
        static id_type matricesId = nameHash("model matrix buffer");
        static id_type entityBufferId = nameHash("entity id buffer");
        static id_type flipbookBufferId = nameHash("flipbook frame buffer");
        static id_type mainId = nameHash("main");
        static auto modelHandle = rendering::ModelCache::create_model("Cube", fs::view("assets://models/cube.glb"));

        if (filter.empty())
            return;

        auto fbo = getFramebuffer(mainId);
        if (!fbo)
        {
            log::error("Main frame buffer is missing.");
            abort();
            return;
        }

        app::context_guard guard(context);

        auto [valid, message] = fbo->verify();
        if (!valid)
        {
            log::error("Main frame buffer isn't complete: {}", message);
            abort();
            return;
        }

        auto material = filter.at(0).get_component<skybox_renderer>()->material;

        const model& mesh = modelHandle.get_model();

        buffer* modelMatrixBuffer = get_meta<buffer>(matricesId);
        if (!modelMatrixBuffer)
            return;

        buffer* entityIdBuffer = get_meta<buffer>(entityBufferId);
        if (!entityIdBuffer)
            return;

        buffer* flipbookBuffer = get_meta<buffer>(flipbookBufferId);
        if (!flipbookBuffer)
            return;

        if (!mesh.buffered)
        {
            modelHandle.init_model_data();
            modelHandle.buffer_data(*modelMatrixBuffer, SV_MODELMATRIX + 0, 4, GL_FLOAT, false, sizeof(math::mat4), 0 * sizeof(math::mat4::col_type), true);
            modelHandle.buffer_data(*modelMatrixBuffer, SV_MODELMATRIX + 1, 4, GL_FLOAT, false, sizeof(math::mat4), 1 * sizeof(math::mat4::col_type), true);
            modelHandle.buffer_data(*modelMatrixBuffer, SV_MODELMATRIX + 2, 4, GL_FLOAT, false, sizeof(math::mat4), 2 * sizeof(math::mat4::col_type), true);
            modelHandle.buffer_data(*modelMatrixBuffer, SV_MODELMATRIX + 3, 4, GL_FLOAT, false, sizeof(math::mat4), 3 * sizeof(math::mat4::col_type), true);

            modelHandle.buffer_data(*entityIdBuffer, SV_ENTITYID, 2, GL_UNSIGNED_INT, false, 0, 0, true);
        }

        entityIdBuffer->bufferData(std::vector<id_type>{filter.at(0)->id});
        modelMatrixBuffer->bufferData(std::vector<math::mat4>{math::mat4(1.f)});

        glDisable(GL_DEPTH_TEST);

        fbo->bind();

        uint attachments[1] = { FRAGMENT_ATTACHMENT };
        glDrawBuffers(1, attachments);

        camInput.bind(material);
        material.bind();
        mesh.vertexArray.bind();
        mesh.indexBuffer.bind();

        glDepthMask(GL_FALSE);

        for (auto submesh : mesh.submeshes)
            glDrawElementsInstanced(GL_TRIANGLES, (GLuint)submesh.indexCount, GL_UNSIGNED_INT, (GLvoid*)(submesh.indexOffset * sizeof(uint)), (GLsizei)1);

        glDepthMask(GL_TRUE);

        mesh.indexBuffer.release();
        mesh.vertexArray.release();
        material.release();

        uint defaultAttachments[4] = { FRAGMENT_ATTACHMENT, NORMAL_ATTACHMENT, POSITION_ATTACHMENT, OVERDRAW_ATTACHMENT };
        glDrawBuffers(4, defaultAttachments);

        fbo->release();
        glEnable(GL_DEPTH_TEST);
    }

    priority_type Skybox::priority()
    {
        return setup_priority - 1;
    }
}
