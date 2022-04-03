#include <rendering/data/material.hpp>

namespace legion::rendering
{
    material_parameter_base* material_parameter_base::create_param(const std::string& name, const GLint& location, const GLenum& type)
    {
        switch (type)
        {
        case GL_SAMPLER_2D_ARRAY:
            return new material_parameter<texture_handle>(name, location);
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

    id_type material_parameter_base::glid_to_typeid(const GLenum& type)
    {
        switch (type)
        {
        case GL_SAMPLER_2D_ARRAY:
            return typeHash<texture_handle>();
        case GL_SAMPLER_2D:
            return typeHash<texture_handle>();
        case GL_FLOAT:
            return typeHash<float>();
            break;
        case GL_FLOAT_VEC2:
            return typeHash<math::vec2>();
            break;
        case GL_FLOAT_VEC3:
            return typeHash<math::vec3>();
            break;
        case GL_FLOAT_VEC4:
            return typeHash<math::vec4>();
            break;
        case GL_UNSIGNED_INT:
            return typeHash<uint>();
            break;
        case GL_INT:
            return typeHash<int>();
            break;
        case GL_INT_VEC2:
            return typeHash<math::ivec2>();
            break;
        case GL_INT_VEC3:
            return typeHash<math::ivec3>();
            break;
        case GL_INT_VEC4:
            return typeHash<math::ivec4>();
            break;
        case GL_BOOL:
            return typeHash<bool>();
            break;
        case GL_BOOL_VEC2:
            return typeHash<math::bvec2>();
            break;
        case GL_BOOL_VEC3:
            return typeHash<math::bvec3>();
            break;
        case GL_BOOL_VEC4:
            return typeHash<math::bvec4>();
            break;
        case GL_FLOAT_MAT2:
            return typeHash<math::mat2>();
            break;
        case GL_FLOAT_MAT3:
            return typeHash<math::mat3>();
            break;
        case GL_FLOAT_MAT4:
            return typeHash<math::mat4>();
            break;
        default:
            return invalid_id;
        }
    }

    async::rw_spinlock MaterialCache::m_materialLock;
    std::unordered_map<id_type, material> MaterialCache::m_materials;
    material_handle MaterialCache::m_invalid_material;

    material_handle MaterialCache::create_material(const std::string& name, const shader_handle& shader)
    {
        id_type id = nameHash(name);

        {
            async::readonly_guard guard(MaterialCache::m_materialLock);

#if defined(LEGION_VALIDATE)
            if (!m_materials.count(invalid_id))
            {
                auto& mat = m_materials[invalid_id];
                mat.init(ShaderCache::get_handle(invalid_id));
                mat.m_name = "invalid";
            }
#endif

            if (m_materials.count(id))
                return { id };

            auto& mat = m_materials[id];
            mat.init(shader);
            mat.m_name = name;
        }

        log::debug("Created material {} with shader: {}", name, shader.get_name());

        return { id };
    }

    material_handle MaterialCache::create_material(const std::string& name, const filesystem::view& shaderFile, shader_import_settings settings)
    {
        id_type id = nameHash(name);

        {
            async::readonly_guard guard(MaterialCache::m_materialLock);

#if defined(LEGION_VALIDATE)
            if (!m_materials.count(invalid_id))
            {
                auto& mat = m_materials[invalid_id];
                mat.init(ShaderCache::get_handle(invalid_id));
                mat.m_name = "invalid";
            }
#endif

            if (m_materials.count(id))
                return { id };
        }

        auto shader = ShaderCache::create_shader(shaderFile, settings);

        {
            async::readonly_guard guard(MaterialCache::m_materialLock);

            auto& mat = m_materials[id];
            mat.init(shader);
            mat.m_name = name;
        }

        log::debug("Created material {} with shader: {}", name, shader.get_name());

        return { id };
    }

    std::pair<async::rw_spinlock&, std::unordered_map<id_type, material>&> MaterialCache::get_all_materials()
    {
        return std::make_pair(std::ref(m_materialLock), std::ref(m_materials));
    }

    void MaterialCache::delete_material(const std::string& name)
    {
        log::debug("Destroyed material {}", name);
        auto id = nameHash(name);

        async::readwrite_guard guard(MaterialCache::m_materialLock);
        m_materials.erase(id);
    }

    void MaterialCache::delete_material(id_type id)
    {
        log::debug("Destroyed material with id: {}", id);
        async::readwrite_guard guard(MaterialCache::m_materialLock);
        m_materials.erase(id);
    }

    material_handle MaterialCache::get_material(const std::string& name)
    {
        id_type id = nameHash(name);
        return get_material(id);
    }

    material_handle MaterialCache::get_material(id_type materialId)
    {
        async::readonly_guard guard(MaterialCache::m_materialLock);

#if defined(LEGION_VALIDATE)
        if (!m_materials.count(invalid_id))
        {
            auto& mat = m_materials[invalid_id];
            mat.init(ShaderCache::get_handle(invalid_id));
            mat.m_name = "invalid";
        }
#endif

        if (m_materials.count(materialId))
            return { materialId };
        return invalid_material_handle;
    }

    id_type material_handle::current_variant() const
    {
        async::readonly_guard guard(MaterialCache::m_materialLock);

#if defined(LEGION_VALIDATE)
        if (!MaterialCache::m_materials.count(id))
            return invalid_id;
#endif

        return MaterialCache::m_materials.at(id).current_variant();
    }

    std::vector<std::reference_wrapper<variant_submaterial>> material_handle::get_variants()
    {
        async::readonly_guard guard(MaterialCache::m_materialLock);
        return MaterialCache::m_materials.at(id).get_variants();
    }

    bool material_handle::has_variant(id_type variantId) const
    {
        async::readonly_guard guard(MaterialCache::m_materialLock);

#if defined(LEGION_VALIDATE)
        if (!MaterialCache::m_materials.count(id))
            return false;
#endif

        return MaterialCache::m_materials.at(id).has_variant(variantId);
    }

    bool material_handle::has_variant(const std::string& variant) const
    {
        id_type variantId = nameHash(variant);
        async::readonly_guard guard(MaterialCache::m_materialLock);

#if defined(LEGION_VALIDATE)
        if (!MaterialCache::m_materials.count(id))
            return false;
#endif

        return MaterialCache::m_materials.at(id).has_variant(variantId);
    }

    void material_handle::set_variant(id_type variantId)
    {
        async::readonly_guard guard(MaterialCache::m_materialLock);

#if defined(LEGION_VALIDATE)
        if (!MaterialCache::m_materials.count(id))
            return;
#endif

        MaterialCache::m_materials.at(id).set_variant(variantId);
    }

    void material_handle::set_variant(const std::string& variant)
    {
        id_type variantId = nameHash(variant);
        async::readonly_guard guard(MaterialCache::m_materialLock);

#if defined(LEGION_VALIDATE)
        if (!MaterialCache::m_materials.count(id))
            return;
#endif

        MaterialCache::m_materials.at(id).set_variant(variantId);
    }

    L_NODISCARD shader_handle material_handle::get_shader() const
    {
        async::readonly_guard guard(MaterialCache::m_materialLock);

#if defined(LEGION_VALIDATE)
        if (!MaterialCache::m_materials.count(id))
            return invalid_shader_handle;
#endif

        return MaterialCache::m_materials.at(id).m_shader;
    }

    void material_handle::destroy()
    {
        MaterialCache::delete_material(id);
    }

    void material_handle::bind() const
    {
        async::readonly_guard guard(MaterialCache::m_materialLock);

#if defined(LEGION_VALIDATE)
        if (!MaterialCache::m_materials.count(id))
        {
            log::error("Material with id {} does not exist!", id);
            return;
        }
#endif

        MaterialCache::m_materials.at(id).bind();
    }

    L_NODISCARD const std::string& material_handle::get_name() const
    {
        async::readonly_guard guard(MaterialCache::m_materialLock);

#if defined(LEGION_VALIDATE)
        if (!MaterialCache::m_materials.count(id))
        {
            static const std::string invalidName = "invalid";
            return invalidName;
        }
#endif

        return MaterialCache::m_materials.at(id).get_name();
    }


    L_NODISCARD const std::unordered_map<id_type, std::unique_ptr<material_parameter_base>>& material_handle::get_params() const
    {
        async::readonly_guard guard(MaterialCache::m_materialLock);

#if defined(LEGION_VALIDATE)
        if (!MaterialCache::m_materials.count(id))
        {
            static const std::unordered_map<id_type, std::unique_ptr<material_parameter_base>> invalidParams;
            return invalidParams;
        }
#endif

        return MaterialCache::m_materials.at(id).get_params();
    }

    attribute material_handle::get_attribute(const std::string& name) const
    {
        async::readonly_guard guard(MaterialCache::m_materialLock);

#if defined(LEGION_VALIDATE)
        if (!MaterialCache::m_materials.count(id))
        {
            log::error("Material with id {} does not exist!", id);
            return invalid_attribute;
        }
#endif

        return MaterialCache::m_materials.at(id).m_shader.get_attribute(name);
    }

    std::vector<std::reference_wrapper<variant_submaterial>> material::get_variants()
    {
        std::vector<std::reference_wrapper<variant_submaterial>> variants;
        variants.reserve(m_variants.size());
        for (auto& [id, variant] : m_variants)
            variants.push_back(std::ref(variant));

        return variants;
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
#if defined(LEGION_VALIDATE)
        if (!m_shader.has_variant(variantId))
            log::error("Shader {} does not have a variant with id {}", m_shader.get_name(), variantId);
#endif
        if (variantId == 0)
        {
            static id_type defaultId = nameHash("default");
            m_currentVariant = defaultId;
        }
        else
            m_currentVariant = variantId;
    }

    void material::set_variant(const std::string& variant)
    {
        std::string variantName = variant;
        std::replace(variantName.begin(), variantName.end(), ' ', '_');
        id_type variantId = nameHash(variantName);

#if defined(LEGION_VALIDATE)
        if (!m_shader.has_variant(variantId))
            log::error("Shader {} does not have a variant named {}", m_shader.get_name(), variantName);
#endif

        m_currentVariant = variantId;
    }

    void material::bind()
    {
#if defined(LEGION_VALIDATE)
        if (!m_shader.is_valid() || !m_shader.has_variant(m_currentVariant) || !m_variants.count(m_currentVariant))
        {
            invalid_shader_handle.bind();
            return;
        }
#endif

        m_shader.configure_variant(m_currentVariant);
        m_shader.bind();
        for (auto& [_, param] : m_variants.at(m_currentVariant).parameters)
            param->apply(m_shader);
    }
}
