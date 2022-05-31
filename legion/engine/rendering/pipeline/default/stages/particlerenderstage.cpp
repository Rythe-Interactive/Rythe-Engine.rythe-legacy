#include <rendering/pipeline/default/stages/particlerenderstage.hpp>
#include <rendering/components/light.hpp>
#include <rendering/data/buffer.hpp>
#include <rendering/data/model.hpp>
#include <rendering/components/renderable.hpp>
#include <rendering/util/bindings.hpp>


namespace legion::rendering
{
    void ParticleRenderStage::setup(app::window& context)
    {
        app::context_guard guard(context);
        if (!guard.contextIsValid())
            return;

        m_defaultDepthOnlyMaterial = MaterialCache::create_material("depth only", fs::view("engine://shaders/depthonly.shs"));
    }

    void ParticleRenderStage::render(app::window& context, camera& cam, const camera::camera_input& camInput, time::span deltaTime)
    {
        (void)deltaTime;
        (void)cam;
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

        //core::time::stopwatch particleRenderWatch;
        //particleRenderWatch.start();

        auto* batches = get_meta< sparse_map < material_handle, sparse_map < model_handle, std::pair<ecs::entity, std::vector<float>>>>>(batchesId);
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

        auto [valid, message] = fbo->verify();
        if (!valid)
        {
            log::error("Main frame buffer isn't complete: {}", message);
            abort();
            return;
        }


        fbo->bind();

        for (auto [material, instancesPerMaterial] : *batches)
        {
            auto shader = material.get_shader();
            if (!shader.is_valid() || !shader.has_variant(material.current_variant()))
                material = invalid_material_handle;

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
                    log::warn("Invalid mesh found in particle emitter");
                    continue;
                }

                ModelCache::create_model(modelHandle.id);
                auto modelName = ModelCache::get_model_name(modelHandle.id);

                const model& mesh = modelHandle.get_model();

                auto& emitter = instances.first.get_component<particle_emitter>().get();

                if (!mesh.buffered)//Binds the modelMatrixBuffer to the modelHandles data
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

                positionBuffer->bufferData(emitter.get_buffer<math::vec4>("posBuffer"));
                orientationBuffer->bufferData(emitter.get_buffer<math::vec4>("rotBuffer"));
                scaleBuffer->bufferData(emitter.get_buffer<math::vec4>("scaleBuffer"));
                flipbookBuffer->bufferData(instances.second);

                if (mesh.submeshes.empty())
                {
                    log::warn("Empty mesh found. Model name: {},  Model ID {}", modelName, modelHandle.get_mesh().id());
                    continue;
                }

                {
                    mesh.vertexArray.bind();
                    mesh.indexBuffer.bind();
                    lightsBuffer->bind();
                    for (auto submesh : mesh.submeshes)
                    {
                        glDrawElementsInstanced(GL_TRIANGLES, (GLuint)submesh.indexCount, GL_UNSIGNED_INT, (GLvoid*)(submesh.indexOffset * sizeof(uint)), (GLsizei)instances.second.size());
                    }
                    lightsBuffer->release();
                    mesh.indexBuffer.release();
                    mesh.vertexArray.release();
                }
            }

            material.release();
        }

        for (auto [m, instancesPerMaterial] : *batches)
        {

            material_handle material = m;

            auto currentVariant = material.current_variant();
            auto shader = material.get_shader();
            if (!shader.is_valid() || !shader.has_variant(depthOnlyVariant))
                material = m_defaultDepthOnlyMaterial;
            else
                material.set_variant(depthOnlyVariant);

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
                    log::warn("Invalid mesh found in particle emitter");
                    continue;
                }

                ModelCache::create_model(modelHandle.id);
                auto modelName = ModelCache::get_model_name(modelHandle.id);

                const model& mesh = modelHandle.get_model();
                auto& emitter = instances.first.get_component<particle_emitter>().get();

                if (!mesh.buffered)//Binds the modelMatrixBuffer to the modelHandles data
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

                positionBuffer->bufferData(emitter.get_buffer<math::vec4>("posBuffer"));
                orientationBuffer->bufferData(emitter.get_buffer<math::vec4>("rotBuffer"));
                scaleBuffer->bufferData(emitter.get_buffer<math::vec4>("scaleBuffer"));
                flipbookBuffer->bufferData(instances.second);

                if (mesh.submeshes.empty())
                {
                    log::warn("Empty mesh found. Model name: {},  Model ID {}", modelName, modelHandle.get_mesh().id());
                    continue;
                }

                {
                    mesh.vertexArray.bind();
                    mesh.indexBuffer.bind();
                    lightsBuffer->bind();
                    for (auto submesh : mesh.submeshes)
                    {
                        glDrawElementsInstanced(GL_TRIANGLES, (GLuint)submesh.indexCount, GL_UNSIGNED_INT, (GLvoid*)(submesh.indexOffset * sizeof(uint)), (GLsizei)instances.second.size());
                    }
                    lightsBuffer->release();
                    mesh.indexBuffer.release();
                    mesh.vertexArray.release();
                }

            }
            material.release();
            if (material != m_defaultDepthOnlyMaterial)
                material.set_variant(currentVariant);
        }

        fbo->release();

        //particleRenderWatch.end();
        //log::debug("Particle Render Stage elapsed time:  {}ms", particleRenderWatch.elapsed_time().milliseconds());
    }

    priority_type ParticleRenderStage::priority()
    {
        return default_priority;
    }
}
