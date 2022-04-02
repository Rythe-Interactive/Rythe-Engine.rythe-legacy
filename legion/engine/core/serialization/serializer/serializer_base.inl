#include <core/serialization/serializer/serializer_base.hpp>
#pragma once
#include <core/serialization/util/container_util.hpp>

#define EndReadPropagate(_result, _warnings, _view)                                                 \
        _warnings.insert(_warnings.end(), _result.warnings().begin(), _result.warnings().end());    \
        if (_result.has_error()) { _view.end_read(); return { _result.error(), _warnings }; }

#define EndObjectPropagate(_result, _warnings, _view)                                               \
        _warnings.insert(_warnings.end(), _result.warnings().begin(), _result.warnings().end());    \
        if (_result.has_error()) { _view.end_object(); return { _result.error(), _warnings }; }

namespace legion::core::serialization
{
    template<typename type>
    inline common::result<void, fs_error> serializer<type>::serialize(const void* serializable, serializer_view& s_view, std::string_view name)
    {
        using serializable_type = remove_cvr_t<type>;

        auto* ptr = static_cast<const serializable_type*>(serializable);

        if constexpr (is_serializable_v<serializable_type>)
        {
            return s_view.serialize(std::string(name), *ptr) ? common::success : legion_fs_error("Type was not a primitive serializable type.");
        }
        else if constexpr (is_container_v<serializable_type>)
        {
            return util::serialize_container(*ptr, s_view, name);
        }
        else
        {
            auto refl = make_reflector(*ptr);
            std::vector<std::string> warnings{};

            s_view.start_object(std::string(name));
            s_view.serialize<std::string>("type_name", nameOfType<serializable_type>());

            for (auto& [name, var] : refl.members)
            {
                if (var.is_object)
                {
                    auto _serializer = SerializerRegistry::getSerializer(var.object.typeId);
                    auto result = _serializer->serialize(var.object.data, s_view, name);
                    EndObjectPropagate(result, warnings, s_view);
                }
                else
                {
                    if (!s_view.serialize(std::string(name), var.primitive.data, var.primitive.typeId))
                    {
                        s_view.end_object();
                        return { legion_fs_error("Type was not a primitive serializable type."), warnings };
                    }
                }
            }

            s_view.end_object();
            return { common::success, warnings };
        }
    }

    template<typename type>
    inline common::result<void, fs_error> serializer<type>::deserialize(void* target, serializer_view& s_view, std::string_view name)
    {
        using serializable_type = remove_cvr_t<type>;
        auto* ptr = static_cast<serializable_type*>(target);

        if constexpr (is_serializable_v<serializable_type>)
        {
            auto result = s_view.deserialize<serializable_type>(std::string(name));
            if (result.has_error())
                return { result.error(), result.warnings() };
            else
                new(ptr) serializable_type(result.value());
        }
        else  if constexpr (is_container_v<serializable_type>)
        {
            new(ptr) serializable_type();
            return util::deserialize_container(*ptr, s_view, name);
        }
        else
        {
            new(ptr) serializable_type();
            reflector refl = make_reflector(*ptr);
            std::vector<std::string> warnings{};

            {
                auto result = s_view.start_read(std::string(name));
                PropagateErrors(result, warnings);
            }

            {
                auto result = s_view.deserialize<std::string>("type_name");
                EndReadPropagate(result, warnings, s_view);

                if (*result != nameOfType<serializable_type>())
                {
                    s_view.end_read();
                    return { legion_fs_error("Item of name " + std::string(name) + " is not of type " + std::string(nameOfType<serializable_type>()) + " but of type " + *result + "."), warnings };
                }
            }

            for (auto& [name, var] : refl.members)
            {
                if (var.is_object)
                {
                    auto _serializer = SerializerRegistry::getSerializer(var.object.typeId);
                    auto result = _serializer->deserialize(var.object.data, s_view, name);
                    EndReadPropagate(result, warnings, s_view);
                }
                else
                {
                    auto result = s_view.deserialize(std::string(name), var.primitive.data, var.primitive.typeId);
                    EndReadPropagate(result, warnings, s_view);
                }
            }
            s_view.end_read();

            return { common::success, warnings };
        }
    }
}

#undef EndReadPropagate
#undef EndObjectPropagate
