#include <rendering/pipeline/default/stages/meshrenderstage.hpp>
#include <rendering/components/light.hpp>
#include <rendering/data/buffer.hpp>
#include <rendering/data/model.hpp>
#include <rendering/components/renderable.hpp>

namespace legion::rendering
{
    void MeshRenderStage::setup(app::window& context)
    {
    }

    void MeshRenderStage::render(app::window& context, camera& cam, const camera::camera_input& camInput, time::span deltaTime)
    {
        OPTICK_EVENT();
        (void)deltaTime;
        (void)cam;
        static id_type mainId = nameHash("main");
        static id_type batchesId = nameHash("mesh batches");
        static id_type lightsId = nameHash("light buffer");
        static id_type lightCountId = nameHash("light count");
        static id_type matricesId = nameHash("model matrix buffer");

        // Leave this for later implementation, no time rn. (Glyn)
        // static id_type sceneColorId = nameHash("scene color history");
        // static id_type sceneDepthId = nameHash("scene depth history");

        //auto* batches = get_meta<sparse_map<material_handle, sparse_map<model_handle, std::unordered_set<ecs::entity>>>>(batchesId);
        auto* batches = get_meta<sparse_map<material_handle, sparse_map<model_handle, std::pair<std::vector<ecs::entity>, std::vector<math::mat4>>>>>(batchesId);
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
            if (material.get_name() == "Test")
            {
                for (auto [modelHandle, instances] : instancesPerMaterial)
                {
                    material_handle mater;

                    if (modelHandle.id == invalid_id)
                    {
                        for (auto& ent : instances.first)
                            log::warn("Invalid mesh found on entity {}.", ent->name);

                        continue;
                    }

                    const model& mesh = modelHandle.get_model();
                    for (auto submesh : mesh.submeshes)
                    {
                        if (mesh.materials.empty())
                            mater = material;
                        else if (submesh.materialIndex == -1)
                            continue;
                        else
                            mater = mesh.materials[submesh.materialIndex];

                        OPTICK_EVENT("Rendering material");
                        auto materialName = mater.get_name();
                        OPTICK_TAG("Material", materialName.c_str());

                        camInput.bind(mater);
                        if (mater.has_param<uint>(SV_LIGHTCOUNT))
                            mater.set_param<uint>(SV_LIGHTCOUNT, *lightCount);

                        if (sceneColor && mater.has_param<texture_handle>(SV_SCENECOLOR))
                            mater.set_param<texture_handle>(SV_SCENECOLOR, sceneColor);

                        if (sceneNormal && mater.has_param<texture_handle>(SV_SCENENORMAL))
                            mater.set_param<texture_handle>(SV_SCENENORMAL, sceneNormal);

                        if (scenePosition && mater.has_param<texture_handle>(SV_SCENEPOSITION))
                            mater.set_param<texture_handle>(SV_SCENEPOSITION, scenePosition);

                        if (hdrOverdraw && mater.has_param<texture_handle>(SV_HDROVERDRAW))
                            mater.set_param<texture_handle>(SV_HDROVERDRAW, hdrOverdraw);

                        if (sceneDepth && mater.has_param<texture_handle>(SV_SCENEDEPTH))
                            mater.set_param<texture_handle>(SV_SCENEDEPTH, sceneDepth);

                        if (mater.has_param<texture_handle>("skybox"))
                            mater.set_param("skybox", TextureCache::create_texture("skybox", fs::view("assets://textures/HDRI/park.jpg")));

                        mater.bind();

                        ModelCache::create_model(modelHandle.id);
                        auto modelName = ModelCache::get_model_name(modelHandle.id);
                        OPTICK_EVENT("Rendering instances");
                        OPTICK_TAG("Model", modelName.c_str());

                        if (!mesh.buffered)
                            modelHandle.buffer_data(*modelMatrixBuffer);

                        if (mesh.submeshes.empty())
                        {
                            log::warn("Empty mesh found. Model name: {},  Model ID {}", modelName, modelHandle.get_mesh().id());
                            continue;
                        }

                        {
                            OPTICK_EVENT("Buffering matrices");
                            /*m_matrices.resize(instances.size());
                            int i = 0;
                            for (auto& ent : instances)
                            {
                                m_matrices[i] = transform(ent.get_component_handles<transform>()).get_local_to_world_matrix();
                                i++;
                            }*/

                            modelMatrixBuffer->bufferData(instances.second);
                        }

                        {
                            OPTICK_EVENT("Draw call");
                            mesh.vertexArray.bind();
                            mesh.indexBuffer.bind();
                            lightsBuffer->bind();
                            glDrawElementsInstanced(GL_TRIANGLES, (GLuint)submesh.indexCount, GL_UNSIGNED_INT, (GLvoid*)(submesh.indexOffset * sizeof(uint)), (GLsizei)instances.second.size());

                            lightsBuffer->release();
                            mesh.indexBuffer.release();
                            mesh.vertexArray.release();
                        }

                        mater.release();
                    }
                }
                continue;
            }

            OPTICK_EVENT("Rendering material");
            auto materialName = material.get_name();
            OPTICK_TAG("Material", materialName.c_str());

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
                        log::warn("Invalid mesh found on entity {}.", ent->name);

                    continue;
                }

                ModelCache::create_model(modelHandle.id);
                auto modelName = ModelCache::get_model_name(modelHandle.id);
                OPTICK_EVENT("Rendering instances");
                OPTICK_TAG("Model", modelName.c_str());

                const model& mesh = modelHandle.get_model();

                if (!mesh.buffered)
                    modelHandle.buffer_data(*modelMatrixBuffer);

                if (mesh.submeshes.empty())
                {
                    log::warn("Empty mesh found. Model name: {},  Model ID {}", modelName, modelHandle.get_mesh().id());
                    continue;
                }

                {
                    OPTICK_EVENT("Buffering matrices");
                    /*m_matrices.resize(instances.size());
                    int i = 0;
                    for (auto& ent : instances)
                    {
                        m_matrices[i] = transform(ent.get_component_handles<transform>()).get_local_to_world_matrix();
                        i++;
                    }*/

                    modelMatrixBuffer->bufferData(instances.second);
                }

                {
                    OPTICK_EVENT("Draw call");
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
        }
        fbo->release();
    }

    priority_type MeshRenderStage::priority()
    {
        return opaque_priority;
    }
}
