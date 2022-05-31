#include <rendering/pipeline/default/stages/skybox.hpp>

namespace legion::rendering
{
    void Skybox::setup(app::window& context)
    {
    }

    void Skybox::render(app::window& context, camera& cam, const camera::camera_input& camInput, time::span deltaTime)
    {
        ecs::filter<skybox_renderer> filter;
        static id_type mainId = nameHash("main");
        static id_type batchesId = nameHash("particle batches");
        static id_type lightsId = nameHash("light buffer");
        static id_type lightCountId = nameHash("light count");
        static id_type matricesId = nameHash("model matrix buffer");
        static id_type positionBufferId = nameHash("position buffer");
        static id_type orientationBufferId = nameHash("orientation buffer");
        static id_type scaleBufferId = nameHash("scale buffer");
        static id_type entityBufferId = nameHash("entity id buffer");
        static id_type flipbookBufferId = nameHash("flipbook frame buffer");
        static id_type depthOnlyVariant = nameHash("depth_only");
        static auto modelHandle = rendering::ModelCache::create_model("Cube", fs::view("assets://models/cube.glb"));

        //core::time::stopwatch watch;
        //watch.start();

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

        buffer* positionBuffer = get_meta<buffer>(positionBufferId);
        if (!positionBuffer)
            return;

        buffer* orientationBuffer = get_meta<buffer>(orientationBufferId);
        if (!orientationBuffer)
            return;

        buffer* scaleBuffer = get_meta<buffer>(scaleBufferId);
        if (!scaleBuffer)
            return;

        buffer* flipbookBuffer = get_meta<buffer>(flipbookBufferId);
        if (!flipbookBuffer)
            return;

        if (!mesh.buffered)
        {
            modelHandle.init_model_data();
            modelHandle.init_buffer(*modelMatrixBuffer, SV_MODELMATRIX + 0, 4, GL_FLOAT, false, sizeof(math::mat4), 0 * sizeof(math::mat4::col_type), true);
            modelHandle.init_buffer(*modelMatrixBuffer, SV_MODELMATRIX + 1, 4, GL_FLOAT, false, sizeof(math::mat4), 1 * sizeof(math::mat4::col_type), true);
            modelHandle.init_buffer(*modelMatrixBuffer, SV_MODELMATRIX + 2, 4, GL_FLOAT, false, sizeof(math::mat4), 2 * sizeof(math::mat4::col_type), true);
            modelHandle.init_buffer(*modelMatrixBuffer, SV_MODELMATRIX + 3, 4, GL_FLOAT, false, sizeof(math::mat4), 3 * sizeof(math::mat4::col_type), true);
            modelHandle.init_buffer(*positionBuffer, SV_TEXCOORD1, 3, GL_FLOAT, false, sizeof(float) * 4, 0, true);
            modelHandle.init_buffer(*orientationBuffer, SV_TEXCOORD2, 4, GL_FLOAT, false, sizeof(float) * 4, 0, true);
            modelHandle.init_buffer(*scaleBuffer, SV_TEXCOORD3, 3, GL_FLOAT, false, sizeof(float) * 4, 0, true);
            modelHandle.init_buffer(*flipbookBuffer, SV_FRAMEID, 1, GL_FLOAT, false, sizeof(float), 0, true);
            modelHandle.init_buffer(*entityIdBuffer, SV_ENTITYID, 2, GL_UNSIGNED_INT, false, 0, 0, true);
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
        //watch.end();
        //log::debug("Skybox Stage elapsed time:  {}ms", watch.elapsed_time().milliseconds());
    }

    priority_type Skybox::priority()
    {
        return setup_priority - 1;
    }
}
