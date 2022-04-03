#include <rendering/data/model.hpp>
#include <rendering/data/material.hpp>
#include <map>
#include <string>
#include <fstream>
namespace legion::rendering
{
    sparse_map<id_type, model> ModelCache::m_models;
    async::rw_spinlock ModelCache::m_modelLock;

    async::rw_spinlock ModelCache::m_modelNameLock;
    std::unordered_map<id_type, std::string> ModelCache::m_modelNames;
    bool model_handle::is_buffered() const
    {
        return ModelCache::get_model(id).buffered;
    }

    void model_handle::buffer_data(const buffer& matrixBuffer, const buffer& entityBuffer, const buffer& flipbookBuffer) const
    {
        ModelCache::buffer_model(id, matrixBuffer, entityBuffer,flipbookBuffer);
    }

    void model_handle::overwrite_buffer(buffer& newBuffer, uint bufferID, bool perInstance) const
    {
        ModelCache::overwrite_buffer(id, newBuffer, bufferID, perInstance);
    }

    assets::asset<mesh> model_handle::get_mesh() const
    {
        return ModelCache::get_mesh(id);
    }

    const model& model_handle::get_model() const
    {
        return ModelCache::get_model(id);
    }

    const model& ModelCache::get_model(id_type id)
    {
        async::readonly_guard guard(m_modelLock);
        return m_models[id];
    }

    sparse_map<id_type, model> ModelCache::get_all_models()
    {
        return m_models;
    }

    std::string ModelCache::get_model_name(id_type id)
    {
        async::readonly_guard guard(m_modelNameLock);
        return m_modelNames[id];
    }

    void ModelCache::overwrite_buffer(id_type id, buffer& newBuffer, uint bufferID, bool perInstance)
    {
        //OPTICK_EVENT();
        if (id == invalid_id)
            return;
        //get mesh handle
        auto mesh_handle = assets::get<mesh>(id);
        if (!mesh_handle)
            return;
        //get mesh and lock
        auto& model = m_models[id];
        if (bufferID == SV_COLOR)
        {
            model.vertexArray.setAttribPointer(newBuffer, SV_COLOR, 4, GL_FLOAT, false, 0, 0);
            model.vertexArray.setAttribDivisor(SV_COLOR, perInstance);
        }
    }

    void ModelCache::buffer_model(id_type id, const buffer& matrixBuffer, const buffer& entityBuffer, const buffer& flipbookBuffer)
    {
        if (id == invalid_id)
            return;

        auto mesh_handle = assets::get<mesh>(id);
        if (!mesh_handle)
            return;

        model& model = m_models[id];

        model.vertexArray = vertexarray::generate();
        model.indexBuffer = buffer(GL_ELEMENT_ARRAY_BUFFER, mesh_handle->indices, GL_STATIC_DRAW);

        model.vertexBuffer = buffer(GL_ARRAY_BUFFER, mesh_handle->vertices, GL_STATIC_DRAW);
        model.vertexArray.setAttribPointer(model.vertexBuffer, SV_POSITION, 3, GL_FLOAT, false, 0, 0);

        model.colorBuffer = buffer(GL_ARRAY_BUFFER, mesh_handle->colors, GL_STATIC_DRAW);
        model.vertexArray.setAttribPointer(model.colorBuffer, SV_COLOR, 4, GL_FLOAT, false, 0, 0);

        model.normalBuffer = buffer(GL_ARRAY_BUFFER, mesh_handle->normals, GL_STATIC_DRAW);
        model.vertexArray.setAttribPointer(model.normalBuffer, SV_NORMAL, 3, GL_FLOAT, false, 0, 0);

        model.tangentBuffer = buffer(GL_ARRAY_BUFFER, mesh_handle->tangents, GL_STATIC_DRAW);
        model.vertexArray.setAttribPointer(model.tangentBuffer, SV_TANGENT, 3, GL_FLOAT, false, 0, 0);

        model.uvBuffer = buffer(GL_ARRAY_BUFFER, mesh_handle->uvs, GL_STATIC_DRAW);
        model.vertexArray.setAttribPointer(model.uvBuffer, SV_TEXCOORD0, 2, GL_FLOAT, false, 0, 0);

        model.vertexArray.setAttribPointer(entityBuffer, SV_ENTITYID, 2, GL_UNSIGNED_INT, false, 0, 0);
        model.vertexArray.setAttribDivisor(SV_ENTITYID, 1);

        model.vertexArray.setAttribPointer(flipbookBuffer, SV_FRAMEID, 1, GL_UNSIGNED_INT, false, 0, 0);
        model.vertexArray.setAttribDivisor(SV_FRAMEID, 1);

        model.vertexArray.setAttribPointer(matrixBuffer, SV_MODELMATRIX + 0, 4, GL_FLOAT, false, sizeof(math::mat4), 0 * sizeof(math::mat4::col_type));
        model.vertexArray.setAttribPointer(matrixBuffer, SV_MODELMATRIX + 1, 4, GL_FLOAT, false, sizeof(math::mat4), 1 * sizeof(math::mat4::col_type));
        model.vertexArray.setAttribPointer(matrixBuffer, SV_MODELMATRIX + 2, 4, GL_FLOAT, false, sizeof(math::mat4), 2 * sizeof(math::mat4::col_type));
        model.vertexArray.setAttribPointer(matrixBuffer, SV_MODELMATRIX + 3, 4, GL_FLOAT, false, sizeof(math::mat4), 3 * sizeof(math::mat4::col_type));

        model.vertexArray.setAttribDivisor(SV_MODELMATRIX + 0, 1);
        model.vertexArray.setAttribDivisor(SV_MODELMATRIX + 1, 1);
        model.vertexArray.setAttribDivisor(SV_MODELMATRIX + 2, 1);
        model.vertexArray.setAttribDivisor(SV_MODELMATRIX + 3, 1);

        model.buffered = true;
    }

    model_handle ModelCache::create_model(const std::string& name, const fs::view& file, assets::import_settings<mesh> settings)
    {
        id_type id = nameHash(name);

        {// Check if the model already exists.
            async::readonly_guard guard(m_modelLock);
            if (m_models.contains(id))
                return { id };
        }

        // Check if the file is valid to load.
        if (!file.is_valid() || !file.file_info().is_file)
            return invalid_model_handle;

        // Load the mesh if it wasn't already. (It's called MeshCache for a reason.)
        model model{};

        auto result = assets::load<mesh>(name, file, settings);
        if (!result)
        {
            log::error("Failed to load model {}: ", name, result.error().what());
            return invalid_model_handle;
        }

        if (result.has_warnings())
            for (auto warn : result.warnings())
                log::warn(warn);

        auto& handle = result.value();
        if (!handle->materials.empty())
        {
            for (auto& mat : handle->materials)
            {
                static auto defaultLitShader = ShaderCache::create_shader("default lit", fs::view("engine://shaders/default_lit.shs"));

                material_handle material = MaterialCache::create_material(name + "/" + mat.name, defaultLitShader);

                if(mat.doubleSided && (mat.transparencyMode == transparency_mode::Blend))
                    material.set_variant("double_sided_transparent");
                else if (mat.doubleSided)
                    material.set_variant("double_sided");
                else if (mat.transparencyMode == transparency_mode::Blend)
                    material.set_variant("transparent");

                material.set_param("alphaCutoff", mat.alphaCutoff);

                if (mat.albedoMap)
                {
                    material.set_param("useAlbedoTex", true);
                    material.set_param("albedoTex", TextureCache::create_texture_from_image(mat.albedoMap));
                }
                else
                {
                    material.set_param("useAlbedoTex", false);
                    material.set_param("albedoColor", mat.albedoValue);
                }

                if (mat.metallicRoughnessMap)
                {
                    material.set_param("useMetallicRoughness", true);
                    material.set_param("metallicRoughness", TextureCache::create_texture_from_image(mat.metallicRoughnessMap));
                }
                else
                {
                    material.set_param("useMetallicRoughness", false);

                    if (mat.metallicMap)
                    {
                        material.set_param("useMetallicTex", true);
                        material.set_param("metallicTex", TextureCache::create_texture_from_image(mat.metallicMap));
                    }
                    else
                    {
                        material.set_param("useMetallicTex", false);
                        material.set_param("metallicValue", mat.metallicValue);
                    }

                    if (mat.roughnessMap)
                    {
                        material.set_param("useRoughnessTex", true);
                        material.set_param("roughnessTex", TextureCache::create_texture_from_image(mat.roughnessMap));
                    }
                    else
                    {
                        material.set_param("useRoughnessTex", false);
                        material.set_param("roughnessValue", mat.roughnessValue);
                    }
                }

                if (mat.emissiveMap)
                {
                    material.set_param("useEmissiveTex", true);
                    material.set_param("emissiveTex", TextureCache::create_texture_from_image(mat.emissiveMap));
                }
                else
                {
                    material.set_param("useEmissiveTex", false);
                    material.set_param("emissiveColor", mat.emissiveValue);
                }

                if (mat.normalMap)
                {
                    material.set_param("useNormal", true);
                    material.set_param("normalTex", TextureCache::create_texture_from_image(mat.normalMap));
                }
                else
                {
                    material.set_param("useNormal", false);
                }

                if (mat.aoMap)
                {
                    material.set_param("useAmbientOcclusion", true);
                    material.set_param("ambientOcclusionTex", TextureCache::create_texture_from_image(mat.aoMap));
                }
                else
                {
                    material.set_param("useAmbientOcclusion", false);
                }

                if (mat.heightMap)
                {
                    material.set_param("useHeight", true);
                    material.set_param("heightTex", TextureCache::create_texture_from_image(mat.heightMap));
                }
                else
                {
                    material.set_param("useHeight", false);
                }

                model.materials.push_back(material);
                log::debug("Loaded embedded material {}/{}", name, mat.name);
            }
        }


        for (auto& submeshData : handle->submeshes)
            model.submeshes.push_back(submeshData);

        // The model still needs to be buffered on the rendering thread.
        model.buffered = false;

        { // Insert the model into the model list.
            async::readwrite_guard guard(m_modelLock);
            m_models.insert(id, model);
        }

        {
            async::readwrite_guard guard(m_modelNameLock);
            m_modelNames[id] = name;
        }

        log::debug("Created model {} with mesh: {}", name, handle.name());

        return { id };
    }

    model_handle ModelCache::create_model(const std::string& name)
    {
        id_type id = nameHash(name);

        {// Check if the model already exists.
            async::readonly_guard guard(m_modelLock);
            if (m_models.contains(id))
                return { id };
        }

        model model{};

        // Load the mesh if it wasn't already. (It's called MeshCache for a reason.)
        auto handle = assets::get<mesh>(name);
        if (!handle)
        {
            log::error("Failed to load model {}", name);
            return invalid_model_handle;
        }

        if (!handle->materials.empty())
        {
            for (auto& mat : handle->materials)
            {
                static auto defaultLitShader = ShaderCache::create_shader("default lit", fs::view("engine://shaders/default_lit.shs"));

                material_handle material = MaterialCache::create_material(name + "/" + mat.name, defaultLitShader);

                if (mat.doubleSided && (mat.transparencyMode == transparency_mode::Blend))
                    material.set_variant("double_sided_transparent");
                else if (mat.doubleSided)
                    material.set_variant("double_sided");
                else if (mat.transparencyMode == transparency_mode::Blend)
                    material.set_variant("transparent");

                material.set_param("alphaCutoff", mat.alphaCutoff);

                if (mat.albedoMap)
                {
                    material.set_param("useAlbedoTex", true);
                    material.set_param("albedoTex", TextureCache::create_texture_from_image(mat.albedoMap));
                }
                else
                {
                    material.set_param("useAlbedoTex", false);
                    material.set_param("albedoColor", mat.albedoValue);
                }

                if (mat.metallicRoughnessMap)
                {
                    material.set_param("useMetallicRoughness", true);
                    material.set_param("metallicRoughness", TextureCache::create_texture_from_image(mat.metallicRoughnessMap));
                }
                else
                {
                    material.set_param("useMetallicRoughness", false);

                    if (mat.metallicMap)
                    {
                        material.set_param("useMetallicTex", true);
                        material.set_param("metallicTex", TextureCache::create_texture_from_image(mat.metallicMap));
                    }
                    else
                    {
                        material.set_param("useMetallicTex", false);
                        material.set_param("metallicValue", mat.metallicValue);
                    }

                    if (mat.roughnessMap)
                    {
                        material.set_param("useRoughnessTex", true);
                        material.set_param("roughnessTex", TextureCache::create_texture_from_image(mat.roughnessMap));
                    }
                    else
                    {
                        material.set_param("useRoughnessTex", false);
                        material.set_param("roughnessValue", mat.roughnessValue);
                    }
                }

                if (mat.emissiveMap)
                {
                    material.set_param("useEmissiveTex", true);
                    material.set_param("emissiveTex", TextureCache::create_texture_from_image(mat.emissiveMap));
                }
                else
                {
                    material.set_param("useEmissiveTex", false);
                    material.set_param("emissiveColor", mat.emissiveValue);
                }

                if (mat.normalMap)
                {
                    material.set_param("useNormal", true);
                    material.set_param("normalTex", TextureCache::create_texture_from_image(mat.normalMap));
                }
                else
                {
                    material.set_param("useNormal", false);
                }

                if (mat.aoMap)
                {
                    material.set_param("useAmbientOcclusion", true);
                    material.set_param("ambientOcclusionTex", TextureCache::create_texture_from_image(mat.aoMap));
                }
                else
                {
                    material.set_param("useAmbientOcclusion", false);
                }

                if (mat.heightMap)
                {
                    material.set_param("useHeight", true);
                    material.set_param("heightTex", TextureCache::create_texture_from_image(mat.heightMap));
                }
                else
                {
                    material.set_param("useHeight", false);
                }

                model.materials.push_back(material);
                log::debug("Loaded embedded material {}/{}", name, mat.name);
            }
        }

        for (auto& submeshData : handle->submeshes)
            model.submeshes.push_back(submeshData);


        // The model still needs to be buffered on the rendering thread.
        model.buffered = false;

        { // Insert the model into the model list.
            async::readwrite_guard guard(m_modelLock);
            m_models.insert(id, model);
        }

        {
            async::readwrite_guard guard(m_modelNameLock);
            m_modelNames[id] = name;
        }

        log::trace("Created model {} with mesh: {}", name, handle.name());

        return { id };
    }

    model_handle ModelCache::create_model(const std::string& name, id_type meshId)
    {
        id_type id = nameHash(name);

        {// Check if the model already exists.
            async::readonly_guard guard(m_modelLock);
            if (m_models.contains(id))
                return { id };
        }

        model model{};

        // Load the mesh if it wasn't already. (It's called AssetCache for a reason.)
        auto handle = assets::get<mesh>(meshId);
        if (!handle)
        {
            log::error("Failed to load model {}", name);
            return invalid_model_handle;
        }

        if (!handle->materials.empty())
        {
            for (auto& mat : handle->materials)
            {
                static auto defaultLitShader = ShaderCache::create_shader("default lit", fs::view("engine://shaders/default_lit.shs"));

                material_handle material = MaterialCache::create_material(name + "/" + mat.name, defaultLitShader);

                if (mat.doubleSided && (mat.transparencyMode == transparency_mode::Blend))
                    material.set_variant("double_sided_transparent");
                else if (mat.doubleSided)
                    material.set_variant("double_sided");
                else if (mat.transparencyMode == transparency_mode::Blend)
                    material.set_variant("transparent");

                material.set_param("alphaCutoff", mat.alphaCutoff);

                if (mat.albedoMap)
                {
                    material.set_param("useAlbedoTex", true);
                    material.set_param("albedoTex", TextureCache::create_texture_from_image(mat.albedoMap));
                }
                else
                {
                    material.set_param("useAlbedoTex", false);
                    material.set_param("albedoColor", mat.albedoValue);
                }

                if (mat.metallicRoughnessMap)
                {
                    material.set_param("useMetallicRoughness", true);
                    material.set_param("metallicRoughness", TextureCache::create_texture_from_image(mat.metallicRoughnessMap));
                }
                else
                {
                    material.set_param("useMetallicRoughness", false);

                    if (mat.metallicMap)
                    {
                        material.set_param("useMetallicTex", true);
                        material.set_param("metallicTex", TextureCache::create_texture_from_image(mat.metallicMap));
                    }
                    else
                    {
                        material.set_param("useMetallicTex", false);
                        material.set_param("metallicValue", mat.metallicValue);
                    }

                    if (mat.roughnessMap)
                    {
                        material.set_param("useRoughnessTex", true);
                        material.set_param("roughnessTex", TextureCache::create_texture_from_image(mat.roughnessMap));
                    }
                    else
                    {
                        material.set_param("useRoughnessTex", false);
                        material.set_param("roughnessValue", mat.roughnessValue);
                    }
                }

                if (mat.emissiveMap)
                {
                    material.set_param("useEmissiveTex", true);
                    material.set_param("emissiveTex", TextureCache::create_texture_from_image(mat.emissiveMap));
                }
                else
                {
                    material.set_param("useEmissiveTex", false);
                    material.set_param("emissiveColor", mat.emissiveValue);
                }

                if (mat.normalMap)
                {
                    material.set_param("useNormal", true);
                    material.set_param("normalTex", TextureCache::create_texture_from_image(mat.normalMap));
                }
                else
                {
                    material.set_param("useNormal", false);
                }

                if (mat.aoMap)
                {
                    material.set_param("useAmbientOcclusion", true);
                    material.set_param("ambientOcclusionTex", TextureCache::create_texture_from_image(mat.aoMap));
                }
                else
                {
                    material.set_param("useAmbientOcclusion", false);
                }

                if (mat.heightMap)
                {
                    material.set_param("useHeight", true);
                    material.set_param("heightTex", TextureCache::create_texture_from_image(mat.heightMap));
                }
                else
                {
                    material.set_param("useHeight", false);
                }

                model.materials.push_back(material);
                log::debug("Loaded embedded material {}/{}", name, mat.name);
            }
        }

        for (auto& submeshData : handle->submeshes)
            model.submeshes.push_back(submeshData);

        // The model still needs to be buffered on the rendering thread.
        model.buffered = false;

        { // Insert the model into the model list.
            async::readwrite_guard guard(m_modelLock);
            m_models.insert(id, model);
        }

        {
            async::readwrite_guard guard(m_modelNameLock);
            m_modelNames[id] = name;
        }

        log::trace("Created model {} with mesh: {}", name, handle.name());

        return { id };
    }

    model_handle ModelCache::create_model(id_type id)
    {
        {// Check if the model already exists.
            async::readonly_guard guard(m_modelLock);
            if (m_models.contains(id))
                return { id };
        }

        model model{};

        // Load the mesh if it wasn't already. (It's called MeshCache for a reason.)
        auto handle = assets::get<mesh>(id);
        if (!handle)
        {
            log::error("Failed to load model {}", id);
            return invalid_model_handle;
        }

        auto& meshName = handle.name();

        // Copy the sub-mesh data.
        if (!handle->materials.empty())
        {
            for (auto& mat : handle->materials)
            {
                static auto defaultLitShader = ShaderCache::create_shader("default lit", fs::view("engine://shaders/default_lit.shs"));

                material_handle material = MaterialCache::create_material(meshName + "/" + mat.name, defaultLitShader);

                if (mat.doubleSided && (mat.transparencyMode == transparency_mode::Blend))
                    material.set_variant("double_sided_transparent");
                else if (mat.doubleSided)
                    material.set_variant("double_sided");
                else if (mat.transparencyMode == transparency_mode::Blend)
                    material.set_variant("transparent");

                material.set_param("alphaCutoff", mat.alphaCutoff);

                if (mat.albedoMap)
                {
                    material.set_param("useAlbedoTex", true);
                    material.set_param("albedoTex", TextureCache::create_texture_from_image(mat.albedoMap));
                }
                else
                {
                    material.set_param("useAlbedoTex", false);
                    material.set_param("albedoColor", mat.albedoValue);
                }

                if (mat.metallicRoughnessMap)
                {
                    material.set_param("useMetallicRoughness", true);
                    material.set_param("metallicRoughness", TextureCache::create_texture_from_image(mat.metallicRoughnessMap));
                }
                else
                {
                    material.set_param("useMetallicRoughness", false);

                    if (mat.metallicMap)
                    {
                        material.set_param("useMetallicTex", true);
                        material.set_param("metallicTex", TextureCache::create_texture_from_image(mat.metallicMap));
                    }
                    else
                    {
                        material.set_param("useMetallicTex", false);
                        material.set_param("metallicValue", mat.metallicValue);
                    }

                    if (mat.roughnessMap)
                    {
                        material.set_param("useRoughnessTex", true);
                        material.set_param("roughnessTex", TextureCache::create_texture_from_image(mat.roughnessMap));
                    }
                    else
                    {
                        material.set_param("useRoughnessTex", false);
                        material.set_param("roughnessValue", mat.roughnessValue);
                    }
                }

                if (mat.emissiveMap)
                {
                    material.set_param("useEmissiveTex", true);
                    material.set_param("emissiveTex", TextureCache::create_texture_from_image(mat.emissiveMap));
                }
                else
                {
                    material.set_param("useEmissiveTex", false);
                    material.set_param("emissiveColor", mat.emissiveValue);
                }

                if (mat.normalMap)
                {
                    material.set_param("useNormal", true);
                    material.set_param("normalTex", TextureCache::create_texture_from_image(mat.normalMap));
                }
                else
                {
                    material.set_param("useNormal", false);
                }

                if (mat.aoMap)
                {
                    material.set_param("useAmbientOcclusion", true);
                    material.set_param("ambientOcclusionTex", TextureCache::create_texture_from_image(mat.aoMap));
                }
                else
                {
                    material.set_param("useAmbientOcclusion", false);
                }

                if (mat.heightMap)
                {
                    material.set_param("useHeight", true);
                    material.set_param("heightTex", TextureCache::create_texture_from_image(mat.heightMap));
                }
                else
                {
                    material.set_param("useHeight", false);
                }

                model.materials.push_back(material);
                log::debug("Loaded embedded material {}/{}", meshName, mat.name);
            }
        }

        for (auto& submeshData : handle->submeshes)
            model.submeshes.push_back(submeshData);

        // The model still needs to be buffered on the rendering thread.
        model.buffered = false;

        { // Insert the model into the model list.
            async::readwrite_guard guard(m_modelLock);
            m_models.insert(id, model);
        }

        {
            async::readwrite_guard guard(m_modelNameLock);
            m_modelNames[id] = std::to_string(id);
        }

        log::trace("Created model {} with mesh: {}", id, meshName);

        return { id };
    }

    model_handle ModelCache::create_model(const std::string& name, assets::asset<mesh> mesh)
    {
        id_type id = nameHash(name);

        {// Check if the model already exists.
            async::readonly_guard guard(m_modelLock);
            if (m_models.contains(id))
                return { id };
        }

        model model{};

        // Load the mesh if it wasn't already. (It's called MeshCache for a reason.)
        if (!mesh)
        {
            log::error("Failed to load model {}", name);
            return invalid_model_handle;
        }

        // Copy the sub-mesh data.
        if (!mesh->materials.empty())
        {
            for (auto& mat : mesh->materials)
            {
                static auto defaultLitShader = ShaderCache::create_shader("default lit", fs::view("engine://shaders/default_lit.shs"));

                material_handle material = MaterialCache::create_material(name + "/" + mat.name, defaultLitShader);

                if (mat.doubleSided && (mat.transparencyMode == transparency_mode::Blend))
                    material.set_variant("double_sided_transparent");
                else if (mat.doubleSided)
                    material.set_variant("double_sided");
                else if (mat.transparencyMode == transparency_mode::Blend)
                    material.set_variant("transparent");

                material.set_param("alphaCutoff", mat.alphaCutoff);

                if (mat.albedoMap)
                {
                    material.set_param("useAlbedoTex", true);
                    material.set_param("albedoTex", TextureCache::create_texture_from_image(mat.albedoMap));
                }
                else
                {
                    material.set_param("useAlbedoTex", false);
                    material.set_param("albedoColor", mat.albedoValue);
                }

                if (mat.metallicRoughnessMap)
                {
                    material.set_param("useMetallicRoughness", true);
                    material.set_param("metallicRoughness", TextureCache::create_texture_from_image(mat.metallicRoughnessMap));
                }
                else
                {
                    material.set_param("useMetallicRoughness", false);

                    if (mat.metallicMap)
                    {
                        material.set_param("useMetallicTex", true);
                        material.set_param("metallicTex", TextureCache::create_texture_from_image(mat.metallicMap));
                    }
                    else
                    {
                        material.set_param("useMetallicTex", false);
                        material.set_param("metallicValue", mat.metallicValue);
                    }

                    if (mat.roughnessMap)
                    {
                        material.set_param("useRoughnessTex", true);
                        material.set_param("roughnessTex", TextureCache::create_texture_from_image(mat.roughnessMap));
                    }
                    else
                    {
                        material.set_param("useRoughnessTex", false);
                        material.set_param("roughnessValue", mat.roughnessValue);
                    }
                }

                if (mat.emissiveMap)
                {
                    material.set_param("useEmissiveTex", true);
                    material.set_param("emissiveTex", TextureCache::create_texture_from_image(mat.emissiveMap));
                }
                else
                {
                    material.set_param("useEmissiveTex", false);
                    material.set_param("emissiveColor", mat.emissiveValue);
                }

                if (mat.normalMap)
                {
                    material.set_param("useNormal", true);
                    material.set_param("normalTex", TextureCache::create_texture_from_image(mat.normalMap));
                }
                else
                {
                    material.set_param("useNormal", false);
                }

                if (mat.aoMap)
                {
                    material.set_param("useAmbientOcclusion", true);
                    material.set_param("ambientOcclusionTex", TextureCache::create_texture_from_image(mat.aoMap));
                }
                else
                {
                    material.set_param("useAmbientOcclusion", false);
                }

                if (mat.heightMap)
                {
                    material.set_param("useHeight", true);
                    material.set_param("heightTex", TextureCache::create_texture_from_image(mat.heightMap));
                }
                else
                {
                    material.set_param("useHeight", false);
                }

                model.materials.push_back(material);
                log::debug("Loaded embedded material {}/{}", name, mat.name);
            }
        }

        for (auto& submeshData : mesh->submeshes)
            model.submeshes.push_back(submeshData);

        // The model still needs to be buffered on the rendering thread.
        model.buffered = false;

        { // Insert the model into the model list.
            async::readwrite_guard guard(m_modelLock);
            m_models.insert(id, model);
        }

        {
            async::readwrite_guard guard(m_modelNameLock);
            m_modelNames[id] = name;
        }

        log::trace("Created model {} with mesh: {}", name, mesh.name());

        return { id };
    }

    model_handle ModelCache::create_model(assets::asset<mesh> mesh)
    {
        id_type id = mesh.id();

        {// Check if the model already exists.
            async::readonly_guard guard(m_modelLock);
            if (m_models.contains(id))
                return { id };
        }

        model model{};

        // Load the mesh if it wasn't already. (It's called MeshCache for a reason.)
        if (!mesh)
        {
            log::error("Failed to load model {}", id);
            return invalid_model_handle;
        }

        // Copy the sub-mesh data.
        auto& meshName = mesh.name();

        if (!mesh->materials.empty())
        {
            for (auto& mat : mesh->materials)
            {
                static auto defaultLitShader = ShaderCache::create_shader("default lit", fs::view("engine://shaders/default_lit.shs"));

                material_handle material = MaterialCache::create_material(meshName + "/" + mat.name, defaultLitShader);

                if (mat.doubleSided && (mat.transparencyMode == transparency_mode::Blend))
                    material.set_variant("double_sided_transparent");
                else if (mat.doubleSided)
                    material.set_variant("double_sided");
                else if (mat.transparencyMode == transparency_mode::Blend)
                    material.set_variant("transparent");

                material.set_param("alphaCutoff", mat.alphaCutoff);

                if (mat.albedoMap)
                {
                    material.set_param("useAlbedoTex", true);
                    material.set_param("albedoTex", TextureCache::create_texture_from_image(mat.albedoMap));
                }
                else
                {
                    material.set_param("useAlbedoTex", false);
                    material.set_param("albedoColor", mat.albedoValue);
                }

                if (mat.metallicRoughnessMap)
                {
                    material.set_param("useMetallicRoughness", true);
                    material.set_param("metallicRoughness", TextureCache::create_texture_from_image(mat.metallicRoughnessMap));
                }
                else
                {
                    material.set_param("useMetallicRoughness", false);

                    if (mat.metallicMap)
                    {
                        material.set_param("useMetallicTex", true);
                        material.set_param("metallicTex", TextureCache::create_texture_from_image(mat.metallicMap));
                    }
                    else
                    {
                        material.set_param("useMetallicTex", false);
                        material.set_param("metallicValue", mat.metallicValue);
                    }

                    if (mat.roughnessMap)
                    {
                        material.set_param("useRoughnessTex", true);
                        material.set_param("roughnessTex", TextureCache::create_texture_from_image(mat.roughnessMap));
                    }
                    else
                    {
                        material.set_param("useRoughnessTex", false);
                        material.set_param("roughnessValue", mat.roughnessValue);
                    }
                }

                if (mat.emissiveMap)
                {
                    material.set_param("useEmissiveTex", true);
                    material.set_param("emissiveTex", TextureCache::create_texture_from_image(mat.emissiveMap));
                }
                else
                {
                    material.set_param("useEmissiveTex", false);
                    material.set_param("emissiveColor", mat.emissiveValue);
                }

                if (mat.normalMap)
                {
                    material.set_param("useNormal", true);
                    material.set_param("normalTex", TextureCache::create_texture_from_image(mat.normalMap));
                }
                else
                {
                    material.set_param("useNormal", false);
                }

                if (mat.aoMap)
                {
                    material.set_param("useAmbientOcclusion", true);
                    material.set_param("ambientOcclusionTex", TextureCache::create_texture_from_image(mat.aoMap));
                }
                else
                {
                    material.set_param("useAmbientOcclusion", false);
                }

                if (mat.heightMap)
                {
                    material.set_param("useHeight", true);
                    material.set_param("heightTex", TextureCache::create_texture_from_image(mat.heightMap));
                }
                else
                {
                    material.set_param("useHeight", false);
                }

                model.materials.push_back(material);
                log::debug("Loaded embedded material {}/{}", meshName, mat.name);
            }
        }

        for (auto& submeshData : mesh->submeshes)
            model.submeshes.push_back(submeshData);

        // The model still needs to be buffered on the rendering thread.
        model.buffered = false;

        { // Insert the model into the model list.
            async::readwrite_guard guard(m_modelLock);
            m_models.insert(id, model);
        }

        m_modelNames[id] = meshName;

        log::trace("Created model {} with mesh: {}", id, meshName);

        return { id };
    }

    model_handle ModelCache::get_handle(const std::string& name)
    {
        id_type id = nameHash(name);
        async::readonly_guard guard(m_modelLock);
        if (m_models.contains(id))
            return { id };
        return invalid_model_handle;
    }

    model_handle ModelCache::get_handle(id_type id)
    {
        async::readonly_guard guard(m_modelLock);
        if (m_models.contains(id))
            return { id };
        return invalid_model_handle;
    }

    assets::asset<mesh> ModelCache::get_mesh(id_type id)
    {
        return assets::get<mesh>(id);
    }

    void ModelCache::destroy_model(const std::string& name)
    {
        bool erased = false;
        id_type id = nameHash(name);
        {
            async::readwrite_guard guard(m_modelLock);

            if (!m_models.contains(id))
                return;
            if (assets::exists<mesh>(id))
                assets::destroy<mesh>(id);
            erased = m_models.erase(id);
        }

        if (erased)
            log::debug("Destroyed model {}", name);
    }

    assets::asset<mesh> ModelCache::get_mesh(const std::string& name)
    {
        id_type id = nameHash(name);
        return assets::get<mesh>(id);
    }
}
