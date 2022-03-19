#include <rendering/data/material.hpp>

namespace legion::rendering
{
    material_parameter_base* material_parameter_base::create_param(const std::string& name, const GLint& location, const GLenum& type)
    {
        switch (type)
        {
        case GL_SAMPLER_2D:
            return new material_parameter<texture_handle>(name, location);
        case GL_FLOAT:
            return new material_parameter<float>(name, location);
            break;
        case GL_FLOAT_VEC2:
            return new material_parameter<math::vec2>(name, location);
            break;
        case GL_FLOAT_VEC3:
            return new material_parameter<math::vec3>(name, location);
            break;
        case GL_FLOAT_VEC4:
            return new material_parameter<math::vec4>(name, location);
            break;
        case GL_UNSIGNED_INT:
            return new material_parameter<uint>(name, location);
            break;
        case GL_INT:
            return new material_parameter<int>(name, location);
            break;
        case GL_INT_VEC2:
            return new material_parameter<math::ivec2>(name, location);
            break;
        case GL_INT_VEC3:
            return new material_parameter<math::ivec3>(name, location);
            break;
        case GL_INT_VEC4:
            return new material_parameter<math::ivec4>(name, location);
            break;
        case GL_BOOL:
            return new material_parameter<bool>(name, location);
            break;
        case GL_BOOL_VEC2:
            return new material_parameter<math::bvec2>(name, location);
            break;
        case GL_BOOL_VEC3:
            return new material_parameter<math::bvec3>(name, location);
            break;
        case GL_BOOL_VEC4:
            return new material_parameter<math::bvec4>(name, location);
            break;
        case GL_FLOAT_MAT2:
            return new material_parameter<math::mat2>(name, location);
            break;
        case GL_FLOAT_MAT3:
            return new material_parameter<math::mat3>(name, location);
            break;
        case GL_FLOAT_MAT4:
            return new material_parameter<math::mat4>(name, location);
            break;
        default:
            return nullptr;
        }
    }

    async::rw_spinlock MaterialCache::m_materialLock;
    std::unordered_map<id_type, material> MaterialCache::m_materials;
    material_handle MaterialCache::m_invalid_material;

    material_handle MaterialCache::create_material(const std::string& name, const shader_handle& shader)
    {
        if (!m_materials.count(invalid_id))
        {
            m_materials[invalid_id].init(ShaderCache::get_handle("invalid"));
            m_materials[invalid_id].m_name = "invalid";
        }

        id_type id = nameHash(name);
        if (m_materials.count(id))
            return { id };


        if (shader == invalid_shader_handle)
        {
            log::error("Tried to create a material named {} with an invalid shader.", name);
            return invalid_material_handle;
        }

        m_materials[id].init(shader);

        if (!m_materials[id].m_variants.size())
        {
            m_materials.erase(id);
            return { invalid_id };
        }

        m_materials[id].m_name = name;

        log::debug("Created material {} with shader: {}", name, shader.get_name());

        return { id };
    }

    material_handle MaterialCache::create_material(const std::string& name, const filesystem::view& shaderFile, shader_import_settings settings)
    {
        if (!m_materials.count(invalid_id))
        {
            m_materials[invalid_id].init(ShaderCache::get_handle("invalid"));
            m_materials[invalid_id].m_name = "invalid";
        }

        id_type id = nameHash(name);
        if (m_materials.count(id))
            return { id };

        auto shader = ShaderCache::create_shader(shaderFile, settings);

        if (shader == invalid_shader_handle)
        {
            log::error("Tried to create a material named {} with an invalid shader file: {}.", name, shaderFile.get_virtual_path());
            return invalid_material_handle;
        }

        m_materials[id].init(shader);
        m_materials[id].m_name = name;

        log::debug("Created material {} with shader: {}", name, shader.get_name());

        return { id };
    }

    std::pair<async::rw_spinlock&, std::unordered_map<id_type, material>&> MaterialCache::get_all_materials()
    {
        return std::make_pair(std::ref(m_materialLock), std::ref(m_materials));
    }

    material_handle MaterialCache::get_material(const std::string& name)
    {
        if (!m_materials.count(invalid_id))
        {
            m_materials[invalid_id].init(ShaderCache::get_handle("invalid"));
            m_materials[invalid_id].m_name = "invalid";
        }

        id_type id = nameHash(name);
        async::readonly_guard guard(m_materialLock);
        if (m_materials.count(id))
            return { id };
        return invalid_material_handle;
    }

    void material::make_unsavable()
    {
        m_canLoadOrSave = false;
    }

    id_type material_handle::current_variant() const
    {
        async::readonly_guard guard(MaterialCache::m_materialLock);
        return MaterialCache::m_materials[id].current_variant();
    }

    bool material_handle::has_variant(id_type variantId) const
    {
        async::readonly_guard guard(MaterialCache::m_materialLock);
        return MaterialCache::m_materials[id].has_variant(variantId);
    }

    bool material_handle::has_variant(const std::string& variant) const
    {
        id_type variantId = nameHash(variant);
        async::readonly_guard guard(MaterialCache::m_materialLock);
        return MaterialCache::m_materials[id].has_variant(variantId);
    }

    void material_handle::set_variant(id_type variantId)
    {
        async::readonly_guard guard(MaterialCache::m_materialLock);
        MaterialCache::m_materials[id].set_variant(variantId);
    }

    void material_handle::set_variant(const std::string& variant)
    {
        id_type variantId = nameHash(variant);
        async::readonly_guard guard(MaterialCache::m_materialLock);
        MaterialCache::m_materials[id].set_variant(variantId);
    }

    L_NODISCARD shader_handle material_handle::get_shader()
    {
        async::readonly_guard guard(MaterialCache::m_materialLock);
        return MaterialCache::m_materials[id].m_shader;
    }

    void material_handle::bind()
    {
        async::readonly_guard guard(MaterialCache::m_materialLock);
        MaterialCache::m_materials[id].bind();
    }

    L_NODISCARD const std::string& material_handle::get_name() const
    {
        async::readonly_guard guard(MaterialCache::m_materialLock);
        return MaterialCache::m_materials[id].get_name();
    }


    L_NODISCARD const std::unordered_map<id_type, std::unique_ptr<material_parameter_base>>& material_handle::get_params()
    {
        async::readonly_guard guard(MaterialCache::m_materialLock);
        return MaterialCache::m_materials[id].get_params();
    }

    attribute material_handle::get_attribute(const std::string& name)
    {
        async::readonly_guard guard(MaterialCache::m_materialLock);
        return MaterialCache::m_materials[id].m_shader.get_attribute(name);
    }

    id_type material::current_variant() const
    {
        return m_currentVariant;
    }

    bool material::has_variant(id_type variantId) const
    {
        return m_shader.has_variant(variantId);
    }

    bool material::has_variant(const std::string& variant) const
    {
        return m_shader.has_variant(variant);
    }

    void material::set_variant(id_type variantId)
    {
        if (m_shader.has_variant(variantId))
            m_currentVariant = variantId;
        else
            m_currentVariant = 0;
    }

    void material::set_variant(const std::string& variant)
    {
        std::string variantName = variant;
        std::replace(variantName.begin(), variantName.end(), ' ', '_');
        id_type variantId = nameHash(variantName);
        if (m_shader.has_variant(variantId))
            m_currentVariant = variantId;
        else
            m_currentVariant = 0;
    }

    void material::bind()
    {
        if (!m_shader.is_valid())
        {
            invalid_shader_handle.bind();
            return;
        }

        m_shader.configure_variant(m_currentVariant);
        m_shader.bind();
        for (auto& [_, param] : m_variants[m_currentVariant].parameters)
            param->apply(m_shader);
    }
}
