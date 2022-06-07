#pragma once
#include <rendering/data/shader.hpp>
#include <memory>
#include <core/filesystem/filesystem.hpp>

/**
 * @file material.hpp
 */

namespace legion::rendering
{
    struct material;

    /**@class material_parameter_base
     * @brief material parameter common base
     */
    struct material_parameter_base
    {
        friend struct material;
    private:
        static material_parameter_base* create_param(const std::string& name, const GLint& location, const GLenum& type);

        static id_type glid_to_typeid(const GLenum& type);

    protected:
        std::string m_name;
        id_type m_id;
        id_type m_typeId;
        GLint m_location;

        material_parameter_base(const std::string& name, GLint location, id_type typeId) : m_name(name), m_id(nameHash(name)), m_typeId(typeId), m_location(location) {}

    public:
        /**@brief Get the type hash of the variable type of this parameter.
         */
        id_type type() const { return m_typeId; }

        /**@brief Get the name of the parameter
         */
        L_NODISCARD std::string get_name() const { return m_name; }

        /**@internal
         */
        virtual void apply(shader_handle& shader) LEGION_PURE;
        /**@endinternal
        */
    };

    /**@class material_parameter
     * @brief Uniform parameter of a material
     * @tparam T Variable type of the parameter
     */
    template<typename T>
    struct material_parameter : public material_parameter_base
    {
        friend struct material;
    private:
        T m_value;

        virtual void apply(shader_handle& shader) override
        {
            shader.get_uniform<T>(m_id).set_value(m_value);
        }
    public:
        material_parameter(const std::string& name, GLint location) : material_parameter_base(name, location, typeHash<T>()), m_value() {}

        void set_value(const T& value) { m_value = value; }
        T get_value() const { return m_value; }
    };

    struct variant_submaterial
    {
        std::string name;
        std::unordered_map<id_type, std::unique_ptr<material_parameter_base>> parameters;
        std::unordered_map<GLint, id_type> idOfLocation;
    };

    /**@class material
     * @brief Structure that keeps track of the parameterization of a certain shader.
     *        Multiple material instances can use the same shader but use different parameter values.
     */
    struct material
    {
        friend class MaterialCache;
        friend struct material_handle;
    private:
        shader_handle m_shader;

        void init(const shader_handle& shader)
        {
            m_shader = shader;
            for (auto& [variantId, variantInfo] : m_shader.get_uniform_info())
            {
                auto& variant = m_variants[variantId];
                variant.name = m_shader.get_variant(variantId).name;
                for (auto& [name, location, type] : variantInfo)
                {
                    id_type hash = nameHash(name);

                    material_parameter_base* existingParam = nullptr;

                    if (variant.parameters.count(hash) && variant.parameters.at(hash)->m_typeId == material_parameter_base::glid_to_typeid(type))
                    {
                        existingParam = variant.parameters.at(hash).get();
                    }

                    if (variant.idOfLocation.count(location) && variant.parameters.at(variant.idOfLocation.at(location))->m_typeId == material_parameter_base::glid_to_typeid(type))
                    {
                        auto tmpHash = variant.idOfLocation.at(location);
                        existingParam = variant.parameters.at(tmpHash).release();
                        variant.parameters.erase(tmpHash);
                    }

                    if (existingParam)
                    {
                        existingParam->m_id = hash;
                        existingParam->m_location = location;
                        existingParam->m_name = name;
                        existingParam->m_typeId = material_parameter_base::glid_to_typeid(type);

                        if(!variant.parameters.count(hash))
                            variant.parameters.emplace(hash, existingParam);

                        variant.idOfLocation[location] = hash;
                    }
                    else
                    {
                        variant.parameters.emplace(hash, material_parameter_base::create_param(name, location, type));
                        variant.idOfLocation.emplace(location, hash);
                    }
                }
            }
        }

        std::string m_name;
        id_type m_currentVariant = nameHash("default");
        std::unordered_map<id_type, variant_submaterial> m_variants;
    public:

        std::vector<std::reference_wrapper<variant_submaterial>> get_variants();

        void reload() { init(m_shader); }

        id_type current_variant() const;

        bool has_variant(id_type variantId) const;
        bool has_variant(const std::string& variant) const;
        void set_variant(id_type variantId);
        void set_variant(const std::string& variant);

        /**@brief Bind the material to the rendering context and prepare for use.
         */
        void bind();

        /**@brief Release the material from the rendering context.
         */
        void release()
        {
            shader_handle::release();
        }

        /**@brief Set the value of a parameter by name.
         */
        template<typename T>
        void set_param(const std::string& name, const T& value);

        /**@brief Check if the material has a parameter by name.
         */
        template<typename T>
        L_NODISCARD bool has_param(const std::string& name);

        /**@brief Get the value of a parameter by name.
         */
        template<typename T>
        L_NODISCARD T get_param(const std::string& name);

        /**@brief Set the value of a parameter by location.
         */
        template<typename T>
        void set_param(GLint location, const T& value);

        /**@brief Check if the material has a parameter by location.
         */
        template<typename T>
        L_NODISCARD bool has_param(GLint location);

        /**@brief Get the value of a parameter by location.
         */
        template<typename T>
        L_NODISCARD T get_param(GLint location);

        /**@brief Get attribute bound to a certain name.
         */
        L_NODISCARD attribute get_attribute(const std::string& name)
        {
            return m_shader.get_attribute(nameHash(name));
        }

        L_NODISCARD const std::string& get_name()
        {
            return m_name;
        }

        L_NODISCARD const std::unordered_map<id_type, std::unique_ptr<material_parameter_base>>& get_params()
        {
            if (m_currentVariant == 0)
                m_currentVariant = nameHash("default");

#if defined(LEGION_VALIDATE)
            if (!m_variants.count(m_currentVariant))
            {
                static const std::unordered_map<id_type, std::unique_ptr<material_parameter_base>> invalidParams;
                return invalidParams;
            }
#endif

            return m_variants.at(m_currentVariant).parameters;
        }
    };

    /**@class material_handle
     * @brief Cheap and safe to pass around handle to a certain material.
     *        Can be treated like a null-able reference, nullify by setting it to invalid_material_handle.
     * @ref material
     */
    struct material_handle
    {
        id_type id;

        id_type current_variant() const;

        std::vector<std::reference_wrapper<variant_submaterial>> get_variants();

        bool has_variant(id_type variantId) const;
        bool has_variant(const std::string& variant) const;
        void set_variant(id_type variantId);
        void set_variant(const std::string& variant);

        L_NODISCARD shader_handle get_shader() const;

        void destroy();

        /**@brief Bind the material to the rendering context and prepare for use.
         */
        void bind() const;

        /**@brief Release the material from the rendering context.
         */
        static void release()
        {
            shader_handle::release();
        }

        /**@brief Set the value of a parameter by name.
         */
        template<typename T>
        void set_param(const std::string& name, const T& value);

        /**@brief Check if the material has a parameter by name.
         */
        template<typename T>
        L_NODISCARD bool has_param(const std::string& name) const;

        /**@brief Get the value of a parameter by name.
         */
        template<typename T>
        L_NODISCARD T get_param(const std::string& name) const;

        /**@brief Set the value of a parameter by location.
         */
        template<typename T>
        void set_param(GLint location, const T& value);

        /**@brief Check if the material has a parameter by location.
         */
        template<typename T>
        L_NODISCARD bool has_param(GLint location) const;

        /**@brief Get the value of a parameter by location.
         */
        template<typename T>
        L_NODISCARD T get_param(GLint location) const;

        L_NODISCARD const std::string& get_name() const;

        L_NODISCARD const std::unordered_map<id_type, std::unique_ptr<material_parameter_base>>& get_params() const;

        /**@brief Get attribute bound to a certain name.
         */
        attribute get_attribute(const std::string& name) const;

        bool operator==(const material_handle& other) const { return id == other.id; }
        bool operator!=(const material_handle& other) const { return id != other.id; }
    };


    /**@brief Default invalid material handle.
     */
    constexpr material_handle invalid_material_handle{ invalid_id };

    /**@class MaterialCache
     * @brief Data cache for creating, storing and managing materials.
     */
    class MaterialCache
    {
        friend struct material_handle;
    private:
        static async::rw_spinlock m_materialLock;
        static std::unordered_map<id_type, material> m_materials;

        static material_handle m_invalid_material;

    public:
        /**@brief Create a new material with a certain name and shader.
         *        If a material already exists with that name it'll return a handle to the already existing material.
         * @return material_handle Handle to the newly created material or the already existing material. Handle may be invalid if the function failed.
         */
        static material_handle create_material(const std::string& name, const shader_handle& shader);

        /**@brief Create a new material with a certain name and shader.
         *        If a new material is created it will also load the shader if it wasn't loaded before using the shader cache.
         *        If a material already exists with that name it'll return a handle to the already existing material.
         * @return material_handle Handle to the newly created material or the already existing material. Handle may be invalid if the function failed.
         * @ref ShaderCache
         */
        static material_handle create_material(const std::string& name, const filesystem::view& shaderFile, shader_import_settings settings = default_shader_settings);

        /**@brief Get a handle to a material with a certain name.
         * @return material_handle Handle to a material attached to the given name, may be invalid if there is no material attached to that name yet.
         */
        static material_handle get_material(const std::string& name);
        static material_handle get_material(id_type materialId);

        static std::pair<async::rw_spinlock&, std::unordered_map<id_type, material>&> get_all_materials();

        static void delete_material(const std::string& name);
        static void delete_material(id_type id);
    };

#pragma region implementations
    template<typename T>
    void material_handle::set_param(const std::string& name, const T& value)
    {
        async::readonly_guard guard(MaterialCache::m_materialLock);

#if defined(LEGION_VALIDATE)
        if (!MaterialCache::m_materials.count(id))
        {
            log::error("Material with id {} does not exist", id);
            return;
        }
#endif

        MaterialCache::m_materials.at(id).set_param<T>(name, value);
    }

    template<typename T>
    void material_handle::set_param(GLint location, const T& value)
    {
        async::readonly_guard guard(MaterialCache::m_materialLock);

#if defined(LEGION_VALIDATE)
        if (!MaterialCache::m_materials.count(id))
        {
            log::error("Material with id {} does not exist", id);
            return;
        }
#endif

        MaterialCache::m_materials.at(id).set_param<T>(location, value);
    }

    template<typename T>
    L_NODISCARD bool material_handle::has_param(const std::string& name) const
    {
        async::readonly_guard guard(MaterialCache::m_materialLock);

#if defined(LEGION_VALIDATE)
        if (!MaterialCache::m_materials.count(id))
        {
            log::error("Material with id {} does not exist", id);
            return false;
        }
#endif

        return MaterialCache::m_materials.at(id).has_param<T>(name);
    }

    template<typename T>
    L_NODISCARD bool material_handle::has_param(GLint location) const
    {
        async::readonly_guard guard(MaterialCache::m_materialLock);

#if defined(LEGION_VALIDATE)
        if (!MaterialCache::m_materials.count(id))
        {
            log::error("Material with id {} does not exist", id);
            return false;
        }
#endif

        return MaterialCache::m_materials.at(id).has_param<T>(location);
    }

    template<typename T>
    L_NODISCARD T material_handle::get_param(const std::string& name) const
    {
        async::readonly_guard guard(MaterialCache::m_materialLock);

#if defined(LEGION_VALIDATE)
        if (!MaterialCache::m_materials.count(id))
        {
            log::error("Material with id {} does not exist", id);
            return T();
        }
#endif

        return MaterialCache::m_materials.at(id).get_param<T>(name);
    }

    template<typename T>
    L_NODISCARD T material_handle::get_param(GLint location) const
    {
        async::readonly_guard guard(MaterialCache::m_materialLock);

#if defined(LEGION_VALIDATE)
        if (!MaterialCache::m_materials.count(id))
        {
            log::error("Material with id {} does not exist", id);
            return T();
        }
#endif

        return MaterialCache::m_materials.at(id).get_param<T>(location);
    }

    template<>
    inline void material::set_param<math::color>(const std::string& name, const math::color& value)
    {
        if (m_currentVariant == 0)
            m_currentVariant = nameHash("default");

#if defined(LEGION_VALIDATE)
        if (!m_variants.count(m_currentVariant))
            m_variants.emplace(m_currentVariant, variant_submaterial());
#endif

        id_type id = nameHash(name);
        variant_submaterial& submaterial = m_variants.at(m_currentVariant);

#if defined(LEGION_VALIDATE)
        if (!submaterial.parameters.count(id) || submaterial.parameters.at(id)->type() != typeHash<math::vec4>())
        {
            log::warn("material {}:{} does not have a parameter named {} of type {}", m_name, submaterial.name, name, nameOfType<math::color>());
            submaterial.parameters.emplace(id, new material_parameter<math::vec4>(name, 0));
        }
#endif

        static_cast<material_parameter<math::vec4>*>(submaterial.parameters.at(id).get())->set_value(value);
    }

    template<>
    L_NODISCARD inline bool material::has_param<math::color>(const std::string& name)
    {
        if (m_currentVariant == 0)
            m_currentVariant = nameHash("default");

#if defined(LEGION_VALIDATE)
        if (!m_variants.count(m_currentVariant))
            return false;
#endif

        id_type id = nameHash(name);
        variant_submaterial& submaterial = m_variants.at(m_currentVariant);
        return submaterial.parameters.count(id) && submaterial.parameters.at(id)->type() == typeHash<math::vec4>();
    }

    template<>
    L_NODISCARD inline math::color material::get_param<math::color>(const std::string& name)
    {
        if (m_currentVariant == 0)
            m_currentVariant = nameHash("default");

#if defined(LEGION_VALIDATE)
        if (!m_variants.count(m_currentVariant))
            return math::color();
#endif

        id_type id = nameHash(name);
        variant_submaterial& submaterial = m_variants.at(m_currentVariant);

#if defined(LEGION_VALIDATE)
        if (!submaterial.parameters.count(id) || submaterial.parameters.at(id)->type() != typeHash<math::vec4>())
        {
            log::warn("material {}:{} does not have a parameter named {} of type {}", m_name, submaterial.name, name, nameOfType<math::color>());
            return math::color();
        }
#endif

        return static_cast<material_parameter<math::vec4>*>(submaterial.parameters.at(id).get())->get_value();
    }

    template<>
    inline void material::set_param<math::color>(GLint location, const math::color& value)
    {
        if (m_currentVariant == 0)
            m_currentVariant = nameHash("default");

#if defined(LEGION_VALIDATE)
        if (!m_variants.count(m_currentVariant))
            m_variants.emplace(m_currentVariant, variant_submaterial());
#endif

        variant_submaterial& submaterial = m_variants.at(m_currentVariant);

#if defined(LEGION_VALIDATE)
        if (!submaterial.idOfLocation.count(location))
        {
            log::warn("material {}:{} does not have a parameter at location {} of type {}", m_name, submaterial.name, location, nameOfType<math::color>());
            submaterial.idOfLocation.emplace(location, nameHash("location " + std::to_string(location)));
        }
#endif

        id_type id = submaterial.idOfLocation.at(location);

#if defined(LEGION_VALIDATE)
        if (!submaterial.parameters.count(id) || submaterial.parameters.at(id)->type() != typeHash<math::vec4>())
        {
            log::warn("material {}:{} does not have a parameter at location {} of type {}", m_name, submaterial.name, location, nameOfType<math::color>());
            submaterial.parameters.emplace(id, new material_parameter<math::vec4>("location " + std::to_string(location), location));
        }
#endif

        static_cast<material_parameter<math::vec4>*>(submaterial.parameters.at(id).get())->set_value(value);
    }

    template<>
    L_NODISCARD inline math::color material::get_param<math::color>(GLint location)
    {
        if (m_currentVariant == 0)
            m_currentVariant = nameHash("default");

#if defined(LEGION_VALIDATE)
        if (!m_variants.count(m_currentVariant))
            return math::color();
#endif

        variant_submaterial& submaterial = m_variants.at(m_currentVariant);

#if defined(LEGION_VALIDATE)
        if (!submaterial.idOfLocation.count(location))
        {
            log::warn("material {}:{} does not have a parameter at location {} of type {}", m_name, submaterial.name, location, nameOfType<math::color>());
            return math::color();
        }
#endif

        id_type id = submaterial.idOfLocation.at(location);

#if defined(LEGION_VALIDATE)
        if (!submaterial.parameters.count(id) || submaterial.parameters.at(id)->type() != typeHash<math::vec4>())
        {
            log::warn("material {}:{} does not have a parameter at location {} of type {}", m_name, submaterial.name, location, nameOfType<math::color>());
            return math::color();
        }
#endif

        return static_cast<material_parameter<math::vec4>*>(submaterial.parameters.at(id).get())->get_value();
    }

    template<>
    L_NODISCARD inline bool material::has_param<math::color>(GLint location)
    {
        if (m_currentVariant == 0)
            m_currentVariant = nameHash("default");

#if defined(LEGION_VALIDATE)
        if (!m_variants.count(m_currentVariant))
            return false;
#endif

        variant_submaterial& submaterial = m_variants.at(m_currentVariant);
        if (!submaterial.idOfLocation.count(location))
            return false;

        id_type id = submaterial.idOfLocation.at(location);
        return submaterial.parameters.count(id) && submaterial.parameters.at(id)->type() == typeHash<math::vec4>();
    }

    template<typename T>
    void material::set_param(const std::string& name, const T& value)
    {
        if (m_currentVariant == 0)
            m_currentVariant = nameHash("default");

#if defined(LEGION_VALIDATE)
        if (!m_variants.count(m_currentVariant))
            m_variants.emplace(m_currentVariant, variant_submaterial());
#endif

        variant_submaterial& submaterial = m_variants.at(m_currentVariant);
        id_type id = nameHash(name);

#if defined(LEGION_VALIDATE)
        if (!submaterial.parameters.count(id) || submaterial.parameters.at(id)->type() != typeHash<T>())
        {
            log::warn("material {}:{} does not have a parameter named {} of type {}", m_name, submaterial.name, name, nameOfType<T>());
            submaterial.parameters.emplace(id, new material_parameter<T>(name, 0));
        }
#endif

        static_cast<material_parameter<T>*>(submaterial.parameters.at(id).get())->set_value(value);
    }

    template<typename T>
    L_NODISCARD bool material::has_param(const std::string& name)
    {
        if (m_currentVariant == 0)
            m_currentVariant = nameHash("default");

#if defined(LEGION_VALIDATE)
        if (!m_variants.count(m_currentVariant))
            return false;
#endif

        id_type id = nameHash(name);
        variant_submaterial& submaterial = m_variants.at(m_currentVariant);
        return submaterial.parameters.count(id) && submaterial.parameters.at(id)->type() == typeHash<T>();
    }

    template<typename T>
    L_NODISCARD T material::get_param(const std::string& name)
    {
        if (m_currentVariant == 0)
            m_currentVariant = nameHash("default");

#if defined(LEGION_VALIDATE)
        if (!m_variants.count(m_currentVariant))
            return T();
#endif

        id_type id = nameHash(name);
        variant_submaterial& submaterial = m_variants.at(m_currentVariant);

#if defined(LEGION_VALIDATE)
        if (!submaterial.parameters.count(id) || submaterial.parameters.at(id)->type() != typeHash<T>())
        {
            log::warn("material {}:{} does not have a parameter named {} of type {}", m_name, submaterial.name, name, nameOfType<T>());
            return T();
        }
#endif

        return static_cast<material_parameter<T>*>(submaterial.parameters.at(id).get())->get_value();
    }

    template<typename T>
    void material::set_param(GLint location, const T& value)
    {
        if (m_currentVariant == 0)
            m_currentVariant = nameHash("default");

#if defined(LEGION_VALIDATE)
        if (!m_variants.count(m_currentVariant))
            m_variants.emplace(m_currentVariant, variant_submaterial());
#endif

        variant_submaterial& submaterial = m_variants.at(m_currentVariant);

#if defined(LEGION_VALIDATE)
        if (!submaterial.idOfLocation.count(location))
        {
            log::warn("material {}:{} does not have a parameter at location {} of type {}", m_name, submaterial.name, location, nameOfType<T>());
            submaterial.idOfLocation.emplace(location, nameHash("location " + std::to_string(location)));
        }
#endif

        id_type id = submaterial.idOfLocation.at(location);

#if defined(LEGION_VALIDATE)
        if (!submaterial.parameters.count(id) || submaterial.parameters.at(id)->type() != typeHash<T>())
        {
            log::warn("material {}:{} does not have a parameter at location {} of type {}", m_name, submaterial.name, location, nameOfType<T>());
            submaterial.parameters.emplace(id, new material_parameter<T>("location " + std::to_string(location), location));
        }
#endif

        static_cast<material_parameter<T>*>(submaterial.parameters.at(id).get())->set_value(value);
    }

    template<typename T>
    L_NODISCARD T material::get_param(GLint location)
    {
        if (m_currentVariant == 0)
            m_currentVariant = nameHash("default");

#if defined(LEGION_VALIDATE)
        if (!m_variants.count(m_currentVariant))
            return T();
#endif

        variant_submaterial& submaterial = m_variants.at(m_currentVariant);

#if defined(LEGION_VALIDATE)
        if (!submaterial.idOfLocation.count(location))
        {
            log::warn("material {}:{} does not have a parameter at location {} of type {}", m_name, submaterial.name, location, nameOfType<T>());
            return T();
        }
#endif

        id_type id = submaterial.idOfLocation.at(location);
#if defined(LEGION_VALIDATE)
        if (!submaterial.parameters.count(id) || submaterial.parameters.at(id)->type() != typeHash<T>())
        {
            log::warn("material {}:{} does not have a parameter at location {} of type {}", m_name, submaterial.name, location, nameOfType<T>());
            return T();
        }
#endif

        return static_cast<material_parameter<T>*>(submaterial.parameters.at(id).get())->get_value();
    }

    template<typename T>
    L_NODISCARD bool material::has_param(GLint location)
    {
        if (m_currentVariant == 0)
            m_currentVariant = nameHash("default");

#if defined(LEGION_VALIDATE)
        if (!m_variants.count(m_currentVariant))
            return;
#endif

        variant_submaterial& submaterial = m_variants.at(m_currentVariant);
        if (!submaterial.idOfLocation.count(location))
            return false;

        id_type id = submaterial.idOfLocation.at(location);
        return submaterial.parameters.count(id) && submaterial.parameters.at(id)->type() == typeHash<T>();
    }
#pragma endregion
}


#if !defined(DOXY_EXCLUDE)
namespace std
{
    template<>
    struct hash<legion::rendering::material_handle>
    {
        std::size_t operator()(legion::rendering::material_handle const& handle) const noexcept
        {
            return static_cast<std::size_t>(handle.id);
        }
    };
}
#endif
