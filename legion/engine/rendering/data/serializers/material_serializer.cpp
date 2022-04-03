#include <rendering/data/model.hpp>

#define EndReadPropagate(_result, _warnings, _view)                                                 \
        _warnings.insert(_warnings.end(), _result.warnings().begin(), _result.warnings().end());    \
        if (_result.has_error()) { _view.end_read(); return { _result.error(), _warnings }; }

#define EndObjectPropagate(_result, _warnings, _view)                                               \
        _warnings.insert(_warnings.end(), _result.warnings().begin(), _result.warnings().end());    \
        if (_result.has_error()) { _view.end_object(); return { _result.error(), _warnings }; }

#define EndReadIgnoreError(_result, _view) if (_result.has_error()) { _result.mark_handled(); _view.end_read(); return; }

#define EndRead2IgnoreError(_result, _view) if (_result.has_error()) { _result.mark_handled(); _view.end_read(); _view.end_read(); return; }

#define IgnoreError(_result) if (_result.has_error()) { _result.mark_handled(); return; }

namespace legion::core::serialization
{
    namespace detail
    {
        template<typename VecType>
        void serialize_vec(const std::string& name, const VecType& item, serializer_view& view)
        {
            view.start_object(name);

            constexpr size_type length = VecType::length();

            view.serialize("x", item.x);
            if constexpr (length > 1)
                view.serialize("y", item.y);

            if constexpr (length > 2)
                view.serialize("z", item.z);

            if constexpr (length > 3)
                view.serialize("w", item.w);

            view.end_object();
        }

        template<typename MatType>
        void serialize_mat(const MatType& item, serializer_view& view)
        {
            view.start_container("value");

            constexpr size_type length = MatType::length();

            for (size_type i = 0; i < length; i++)
                serialize_vec("", item[i], view);

            view.end_container();
        }

        template<typename VecType>
        void deserialize_vec(const std::string& name, VecType& item, serializer_view& view)
        {
            constexpr size_type length = VecType::length();

            view.start_read(name);

            {
                auto result = view.deserialize<typename VecType::value_type>("x");
                IgnoreError(result);
                item.x = *result;
            }

            if constexpr (length > 1)
            {
                auto result = view.deserialize<typename VecType::value_type>("y");
                IgnoreError(result);
                item.y = *result;
            }

            if constexpr (length > 2)
            {
                auto result = view.deserialize<typename VecType::value_type>("z");
                IgnoreError(result);
                item.z = *result;
            }

            if constexpr (length > 3)
            {
                auto result = view.deserialize<typename VecType::value_type>("w");
                IgnoreError(result);
                item.w = *result;
            }

            view.end_read();
        }

        template<typename MatType>
        void deserialize_mat(const MatType& item, serializer_view& view)
        {
            view.start_container("value");
            constexpr size_type length = MatType::length();

            for (size_type i = 0; i < length; i++)
            {
                deserialize_vec("", item[i], view);
            }
            view.end_container();
        }

        void serialize_material_param(const rendering::material_parameter_base* paramPtr, serializer_view& view)
        {
            using namespace rendering;

            auto type = paramPtr->type();
            std::string name(paramPtr->get_name().c_str());

            if (common::starts_with(name, "lgn_"))
                return;

            view.start_object();


            if (type == typeHash<texture_handle>())
            {
                view.serialize<std::string>("type_name", nameOfType<texture_handle>());
                view.serialize("name", name);
                auto value = static_cast<const material_parameter<texture_handle>*>(paramPtr)->get_value();
                auto& path = value.get_texture().path;
                view.start_object("value");
                if (!path.empty())
                {
                    view.serialize("loaded_from_file", true);
                    view.serialize("path", path);
                }
                else
                {
                    view.serialize("loaded_from_file", false);
                    auto data = value.get_data();
                    detail::serialize_vec("size", data.size, view);
                    view.serialize("type", data.type);
                    view.start_container("pixels");
                    for (auto& color : data.pixels)
                        detail::serialize_vec("", color, view);
                    view.end_container();
                }
                view.end_object();
            }
            else if (type == typeHash<float>())
            {
                view.serialize<std::string>("type_name", nameOfType<float>());
                view.serialize("name", name);
                view.serialize("value", static_cast<const material_parameter<float>*>(paramPtr)->get_value());
            }
            else if (type == typeHash<math::vec2>())
            {
                view.serialize<std::string>("type_name", nameOfType<math::vec2>());
                view.serialize("name", name);
                detail::serialize_vec("value", static_cast<const material_parameter<math::vec2>*>(paramPtr)->get_value(), view);
            }
            else if (type == typeHash<math::vec3>())
            {
                view.serialize<std::string>("type_name", nameOfType<math::vec3>());
                view.serialize("name", name);
                detail::serialize_vec("value", static_cast<const material_parameter<math::vec3>*>(paramPtr)->get_value(), view);

            }
            else if (type == typeHash<math::vec4>())
            {
                view.serialize<std::string>("type_name", nameOfType<math::vec4>());
                view.serialize("name", name);
                detail::serialize_vec("value", static_cast<const material_parameter<math::vec4>*>(paramPtr)->get_value(), view);

            }
            else if (type == typeHash<uint>())
            {
                view.serialize<std::string>("type_name", nameOfType<uint>());
                view.serialize("name", name);
                view.serialize("value", static_cast<const material_parameter<uint>*>(paramPtr)->get_value());

            }
            else if (type == typeHash<int>())
            {
                view.serialize<std::string>("type_name", nameOfType<int>());
                view.serialize("name", name);
                view.serialize("value", static_cast<const material_parameter<int>*>(paramPtr)->get_value());
            }
            else if (type == typeHash<math::ivec2>())
            {
                view.serialize<std::string>("type_name", nameOfType<math::ivec2>());
                view.serialize("name", name);
                detail::serialize_vec("value", static_cast<const material_parameter<math::ivec2>*>(paramPtr)->get_value(), view);

            }
            else if (type == typeHash<math::ivec3>())
            {
                view.serialize<std::string>("type_name", nameOfType<math::ivec3>());
                view.serialize("name", name);
                detail::serialize_vec("value", static_cast<const material_parameter<math::ivec3>*>(paramPtr)->get_value(), view);

            }
            else if (type == typeHash<math::ivec4>())
            {
                view.serialize<std::string>("type_name", nameOfType<math::ivec4>());
                view.serialize("name", name);
                detail::serialize_vec("value", static_cast<const material_parameter<math::ivec4>*>(paramPtr)->get_value(), view);

            }
            else if (type == typeHash<bool>())
            {
                view.serialize<std::string>("type_name", nameOfType<bool>());
                view.serialize("name", name);
                view.serialize("value", static_cast<const material_parameter<bool>*>(paramPtr)->get_value());

            }
            else if (type == typeHash<math::bvec2>())
            {
                view.serialize<std::string>("type_name", nameOfType<math::bvec2>());
                view.serialize("name", name);
                detail::serialize_vec("value", static_cast<const material_parameter<math::bvec2>*>(paramPtr)->get_value(), view);

            }
            else if (type == typeHash<math::bvec3>())
            {
                view.serialize<std::string>("type_name", nameOfType<math::bvec3>());
                view.serialize("name", name);
                detail::serialize_vec("value", static_cast<const material_parameter<math::bvec3>*>(paramPtr)->get_value(), view);

            }
            else if (type == typeHash<math::bvec4>())
            {
                view.serialize<std::string>("type_name", nameOfType<math::bvec4>());
                view.serialize("name", name);
                detail::serialize_vec("value", static_cast<const material_parameter<math::bvec4>*>(paramPtr)->get_value(), view);

            }
            else if (type == typeHash<math::mat2>())
            {
                view.serialize<std::string>("type_name", nameOfType<math::mat2>());
                view.serialize("name", name);
                detail::serialize_mat(static_cast<const material_parameter<math::mat2>*>(paramPtr)->get_value(), view);

            }
            else if (type == typeHash<math::mat3>())
            {
                view.serialize<std::string>("type_name", nameOfType<math::mat3>());
                view.serialize("name", name);
                detail::serialize_mat(static_cast<const material_parameter<math::mat3>*>(paramPtr)->get_value(), view);

            }
            else if (type == typeHash<math::mat4>())
            {
                view.serialize<std::string>("type_name", nameOfType<math::mat4>());
                view.serialize("name", name);
                detail::serialize_mat(static_cast<const material_parameter<math::mat4>*>(paramPtr)->get_value(), view);

            }

            view.end_object();
        }

        void deserialize_material_param(rendering::material_handle material, serializer_view& view)
        {
            using namespace rendering;

            view.start_read("");

            id_type type;

            {
                auto result = view.deserialize<std::string>("type_name");
                EndReadIgnoreError(result, view);

                type = nameHash(*result);
            }

            std::string name;

            {
                auto result = view.deserialize<std::string>("name");
                EndReadIgnoreError(result, view);

                name = *result;
            }


            if (type == typeHash<texture_handle>())
            {
                view.start_read("value");
                texture_handle value;

                bool loadedFromFile;

                {
                    auto result = view.deserialize<bool>("loaded_from_file");
                    EndRead2IgnoreError(result, view);
                    loadedFromFile = *result;
                }

                if (loadedFromFile)
                {
                    std::string path;

                    {
                        auto result = view.deserialize<std::string>("path");
                        EndRead2IgnoreError(result, view);
                        path = *result;
                    }

                    value = TextureCache::create_texture(fs::view(path));
                }
                else
                {
                    //view.serialize("loaded_from_file", false);
                    //auto data = value.get_data();
                    //detail::serialize_vec("size", data.size, view);
                    //view.serialize("type", data.type);
                    //view.start_container("pixels");
                    //for (auto& color : data.pixels)
                    //    detail::serialize_vec("", color, view);
                    //view.end_container();
                }

                material.set_param(name, value);
                view.end_read();
            }
            else if (type == typeHash<float>())
            {
                auto result = view.deserialize<float>("value");
                EndRead2IgnoreError(result, view);
                material.set_param(name, *result);
            }
            else if (type == typeHash<math::vec2>())
            {
                math::vec2 value; detail::deserialize_vec("value", value, view); material.set_param(name, value);
            }
            else if (type == typeHash<math::vec3>())
            {
                math::vec3 value; detail::deserialize_vec("value", value, view); material.set_param(name, value);
            }
            else if (type == typeHash<math::vec4>())
            {
                math::vec4 value; detail::deserialize_vec("value", value, view); material.set_param(name, value);

            }
            else if (type == typeHash<uint>())
            {
                auto result = view.deserialize<uint>("value");
                EndRead2IgnoreError(result, view);
                material.set_param(name, *result);
            }
            else if (type == typeHash<int>())
            {
                auto result = view.deserialize<int>("value");
                EndRead2IgnoreError(result, view);
                material.set_param(name, *result);
            }
            else if (type == typeHash<math::ivec2>())
            {
                math::ivec2 value; detail::deserialize_vec("value", value, view); material.set_param(name, value);

            }
            else if (type == typeHash<math::ivec3>())
            {
                math::ivec3 value; detail::deserialize_vec("value", value, view); material.set_param(name, value);

            }
            else if (type == typeHash<math::ivec4>())
            {
                math::ivec4 value; detail::deserialize_vec("value", value, view); material.set_param(name, value);

            }
            else if (type == typeHash<bool>())
            {
                auto result = view.deserialize<bool>("value");
                EndRead2IgnoreError(result, view);
                material.set_param(name, *result);
            }
            else if (type == typeHash<math::bvec2>())
            {
                math::bvec2 value; detail::deserialize_vec("value", value, view); material.set_param(name, value);

            }
            else if (type == typeHash<math::bvec3>())
            {
                math::bvec3 value; detail::deserialize_vec("value", value, view); material.set_param(name, value);

            }
            else if (type == typeHash<math::bvec4>())
            {
                math::bvec4 value; detail::deserialize_vec("value", value, view); material.set_param(name, value);

            }
            else if (type == typeHash<math::mat2>())
            {
                //detail::serialize_mat(static_cast<const material_parameter<math::mat2>*>(paramPtr)->get_value(), view);
            }
            else if (type == typeHash<math::mat3>())
            {
                //detail::serialize_mat(static_cast<const material_parameter<math::mat3>*>(paramPtr)->get_value(), view);
            }
            else if (type == typeHash<math::mat4>())
            {
                //detail::serialize_mat(static_cast<const material_parameter<math::mat4>*>(paramPtr)->get_value(), view);
            }
            view.end_read();
        }

    }

    template<>
    common::result<void, fs_error> serializer<rendering::material_handle>::serialize(const void* src, serializer_view& view, std::string_view name)
    {
        namespace gfx = rendering;
        std::vector<std::string> warnings{};

        view.start_object(std::string(name));
        view.serialize<std::string>("type_name", nameOfType<gfx::material_handle>());

        auto& ref = *static_cast<const gfx::material_handle*>(src);

        view.serialize("name", ref.get_name());

        auto shader = ref.get_shader();

        view.serialize("shader", shader.get_path());

        view.serialize("variant", shader.get_variant(ref.current_variant()).name);

        auto& params = ref.get_params();

        view.start_container("params");

        for (auto& [id, ptr] : params)
        {
            detail::serialize_material_param(ptr.get(), view);
        }

        view.end_container();

        view.end_object();
        return { common::success, warnings };
    }

    template<>
    common::result<void, fs_error> serializer<rendering::material_handle>::deserialize(void* target, serializer_view& view, std::string_view name)
    {
        namespace gfx = rendering;

        std::vector<std::string> warnings{};

        {
            auto result = view.start_read(std::string(name)); // Object
            PropagateErrors(result, warnings);
        }

        {
            auto result = view.deserialize<std::string>("type_name");
            EndReadPropagate(result, warnings, view);

            if (*result != nameOfType<gfx::material_handle>())
            {
                view.end_read();
                return { legion_fs_error("Item of name " + std::string(name) + " is not of type " + std::string(nameOfType<gfx::material_handle>()) + " but of type " + *result + "."), warnings };
            }
        }

        std::string assetName;

        {
            auto result = view.deserialize<std::string>("name");
            EndReadPropagate(result, warnings, view);
            assetName = *result;
        }

        gfx::shader_handle shader;

        {
            auto result = view.deserialize<std::string>("shader");
            EndReadPropagate(result, warnings, view);
            shader = gfx::ShaderCache::create_shader(fs::view(*result));
        }

        std::string variant;

        {
            auto result = view.deserialize<std::string>("variant");
            EndReadPropagate(result, warnings, view);
            variant = *result;
        }

        auto material = gfx::MaterialCache::create_material(assetName, shader);
        material.set_variant(variant);

        {
            auto result = view.start_read("params");
            EndReadPropagate(result, warnings, view);
        }

        size_type size = view.current_item_size();

        for (size_type i = 0; i < size; i++)
        {
            detail::deserialize_material_param(material, view);
        }

        view.end_read(); // Container
        view.end_read(); // Object

        new (target) gfx::material_handle(material);

        return { common::success, warnings };

    }
}
