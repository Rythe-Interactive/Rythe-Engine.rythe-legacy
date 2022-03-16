#include <rendering/pipeline/default/postfx/skybox.hpp>
#include <rendering/components/renderable.hpp>
namespace legion::rendering
{
    void Skybox::setup(app::window& context)
    {
        addRenderPass<&Skybox::renderPass>();
    }

    void Skybox::renderPass(framebuffer& fbo, RenderPipelineBase* pipeline, camera& cam, const camera::camera_input& camInput, time::span deltaTime)
    {
        ecs::filter<skybox_renderer> filter;
        static id_type matricesId = nameHash("model matrix buffer");
        static auto modelHandle = rendering::ModelCache::create_model("Cube", fs::view("assets://models/cube.glb"));

        if (filter.empty())
            return;

        auto material = filter.at(0).get_component<skybox_renderer>()->material;

        const model& mesh = modelHandle.get_model();

        buffer* modelMatrixBuffer = pipeline->get_meta<buffer>(matricesId);
        if (!modelMatrixBuffer)
            return;

        if (!mesh.buffered)
            modelHandle.buffer_data(*modelMatrixBuffer);

        modelMatrixBuffer->bufferData(std::vector<math::mat4>{math::mat4(1.f)});

        texture_handle sceneDepth;
        auto depthAttachment = fbo.getAttachment(GL_DEPTH_ATTACHMENT);
        if (std::holds_alternative<std::monostate>(depthAttachment))
            depthAttachment = fbo.getAttachment(GL_DEPTH_STENCIL_ATTACHMENT);
        if (std::holds_alternative<texture_handle>(depthAttachment))
            sceneDepth = std::get<texture_handle>(depthAttachment);

        if (sceneDepth && material.has_param<texture_handle>(SV_SCENEDEPTH))
            material.set_param<texture_handle>(SV_SCENEDEPTH, sceneDepth);

        glEnable(GL_DEPTH_TEST);

        fbo.bind();
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
        fbo.release();
        glDisable(GL_DEPTH_TEST);
    }
}
