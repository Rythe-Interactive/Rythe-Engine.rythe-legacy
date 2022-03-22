#include "mousehover.hpp"
using namespace legion;

void MouseHover::setup(app::window& context)
{
}

void MouseHover::render(app::window& context, gfx::camera& cam, const gfx::camera::camera_input& camInput, time::span deltaTime)
{
    using namespace rendering;
    (void)deltaTime;
    (void)cam;
    static id_type batchesId = nameHash("mesh batches");
    static id_type matricesId = nameHash("model matrix buffer");

    auto* batches = get_meta<sparse_map<material_handle, sparse_map<model_handle, std::pair<std::vector<ecs::entity>, std::vector<math::mat4>>>>>(batchesId);
    if (!batches)
        return;

    buffer* modelMatrixBuffer = get_meta<buffer>(matricesId);
    if (!modelMatrixBuffer)
        return;

    app::context_guard guard(context);
    if (!guard.contextIsValid())
    {
        abort();
        return;
    }

    m_fbo.bind();

    for (auto [m, instancesPerMaterial] : *batches)
    {
        if (m.get_name() == "Test")
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
                        mater = m;
                    else if (submesh.materialIndex == -1)
                        continue;
                    else
                        mater = mesh.materials[submesh.materialIndex];

                    auto shader = mater.get_shader();
                    if (!shader.is_valid() || !shader.has_variant(mater.current_variant()))
                        mater = invalid_material_handle;

                    auto materialName = mater.get_name();

                    shader.get_uniform_with_location<math::mat4>(SV_VIEW).set_value(camInput.view);
                    shader.get_uniform_with_location<math::mat4>(SV_PROJECT).set_value(camInput.proj);

                    shader.bind();

                    ModelCache::create_model(modelHandle.id);

                    if (!mesh.buffered)
                        modelHandle. (*modelMatrixBuffer);

                    if (mesh.submeshes.empty())
                    {
                        log::warn("Empty mesh found. Model name: {},  Model ID {}", ModelCache::get_model_name(modelHandle.id), modelHandle.get_mesh().id());
                        continue;
                    }

                    modelMatrixBuffer->bufferData(instances.second);

                    {
                        mesh.vertexArray.bind();
                        mesh.indexBuffer.bind();

                        glDrawElementsInstanced(GL_TRIANGLES, (GLuint)submesh.indexCount, GL_UNSIGNED_INT, (GLvoid*)(submesh.indexOffset * sizeof(uint)), (GLsizei)instances.second.size());

                        mesh.indexBuffer.release();
                        mesh.vertexArray.release();
                    }

                    mater.release();
                }
            }
            continue;
        }

        material_handle material = m;
        auto shader = material.get_shader();
        if (!shader.is_valid() || !shader.has_variant(material.current_variant()))
            material = invalid_material_handle;

        camInput.bind(material);

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

            const model& mesh = modelHandle.get_model();

            if (!mesh.buffered)
                modelHandle.buffer_data(*modelMatrixBuffer);

            if (mesh.submeshes.empty())
            {
                log::warn("Empty mesh found. Model name: {},  Model ID {}", modelName, modelHandle.get_mesh().id());
                continue;
            }

            modelMatrixBuffer->bufferData(instances.second);

            {
                mesh.vertexArray.bind();
                mesh.indexBuffer.bind();

                for (auto submesh : mesh.submeshes)
                    glDrawElementsInstanced(GL_TRIANGLES, (GLuint)submesh.indexCount, GL_UNSIGNED_INT, (GLvoid*)(submesh.indexOffset * sizeof(uint)), (GLsizei)instances.second.size());

                mesh.indexBuffer.release();
                mesh.vertexArray.release();
            }
        }

        material.release();
    }
    m_fbo.release();
}

priority_type MouseHover::priority()
{
    return default_priority;
}
