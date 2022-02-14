#include <core/ecs/serialization/entity_serializer.hpp>
#pragma once

#include <core/serialization/detail/serialize_container.inl>

#define EndReadPropagate(_result, _warnings, _view)                                                 \
        _warnings.insert(_warnings.end(), _result.warnings().begin(), _result.warnings().end());    \
        if (_result.has_error()) { _view.end_read(); return { _result.error(), _warnings }; }

#define EndObjectPropagate(_result, _warnings, _view)                                               \
        _warnings.insert(_warnings.end(), _result.warnings().begin(), _result.warnings().end());    \
        if (_result.has_error()) { _view.end_object(); return { _result.error(), _warnings }; }

namespace legion::core::serialization
{
    namespace detail
    {
        inline common::result<void, fs_error> serialize_ent_data(const ecs::entity_data& ent_data, serializer_view& s_view, std::string_view name)
        {
            if (!ent_data.alive)
                return legion_fs_error("Entity does not exist.");

            std::vector<std::string> warnings{};

            s_view.start_object(std::string(name));
            s_view.serialize<std::string>("type_name", nameOfType<ecs::entity_data>());
            s_view.serialize("name", ent_data.name);
            s_view.serialize("active", ent_data.active);
            std::vector<ecs::entity> children{ ent_data.children.begin(), ent_data.children.end() };
            auto result = detail::serialize_container(children, s_view, "children");
            EndObjectPropagate(result, warnings, s_view);

            s_view.start_container("components");

            auto ent = ecs::Registry::getEntity(ent_data.id);
            auto ent_composition = ent.component_composition();

            for (id_type typeId : ent_composition)
            {
                auto compName = std::string(type_hash::from_id(typeId).name());
                if (compName.empty())
                {
                    s_view.end_container();
                    s_view.end_object();
                    return { legion_fs_error("Component type " + std::to_string(typeId) + " has no type data."), warnings };
                }

                auto _serializer = SerializerRegistry::getSerializer(typeId);
                if (!_serializer)
                {
                    warnings.push_back("Could not find existing serializer for " + compName);
                    continue;
                }

                auto result = _serializer->serialize(ecs::Registry::getComponent(typeId, ent), s_view, compName);
                warnings.insert(warnings.end(), result.warnings().begin(), result.warnings().end());

                if (result.has_error())
                {
                    s_view.end_container();
                    s_view.end_object();
                    return { result.error(), warnings };
                }
            }

            s_view.end_container();
            s_view.end_object();
            return { common::success, warnings };
        }

        inline common::result<void, fs_error> deserialize_ent_data(void* target, serializer_view& s_view, std::string_view name)
        {
            std::vector<std::string> warnings{};

            {
                auto result = s_view.start_read(std::string(name)); // Object
                PropagateErrors(result, warnings);
            }

            {
                auto result = s_view.deserialize<std::string>("type_name");
                EndReadPropagate(result, warnings, s_view);

                if (*result != nameOfType<ecs::entity_data>())
                {
                    s_view.end_read();
                    return { legion_fs_error("Item of name " + std::string(name) + " is not of type " + std::string(nameOfType<ecs::entity_data>()) + " but of type " + *result + "."), warnings };
                }
            }

            // Entity
            auto ent = ecs::Registry::createEntity();

            {
                auto result = s_view.deserialize<std::string>("name");
                EndReadPropagate(result, warnings, s_view);
                ent->name = *result;
            }

            {
                auto result = s_view.deserialize<bool>("active");
                EndReadPropagate(result, warnings, s_view);
                ent->active = *result;
            }

            // Children
            byte dataBuffer[sizeof(std::vector<ecs::entity>)];
            std::vector<ecs::entity>* children = reinterpret_cast<std::vector<ecs::entity>*>(dataBuffer);
            {
                auto result = deserialize_container<std::vector<ecs::entity>>(children, s_view, "children");
                EndReadPropagate(result, warnings, s_view);
            }

            for (auto& child : *children)
                ent.add_child(child);

            // Components
            {
                auto result = s_view.start_read("components");
                EndReadPropagate(result, warnings, s_view);
            }

            for (size_type i = 0; i < s_view.current_item_size(); i++)
            {
                s_view.peak_ahead("");
                id_type typeId;

                {
                    auto result = s_view.deserialize<std::string>("type_name");
                    warnings.insert(warnings.end(), result.warnings().begin(), result.warnings().end());

                    if (result.has_error())
                    {
                        s_view.end_read(); // Components
                        s_view.end_read(); // Object
                        return { result.error(), warnings };
                    }

                    typeId = nameHash(*result);
                }

                auto _serializer = SerializerRegistry::getSerializer(typeId);

                byte_vec buffer{};
                buffer.resize(_serializer->type_size());

                {
                    auto result = _serializer->deserialize(buffer.data(), s_view, "");

                    warnings.insert(warnings.end(), result.warnings().begin(), result.warnings().end());

                    if (result.has_error())
                    {
                        s_view.end_read(); // Components
                        s_view.end_read(); // Object
                        return { result.error(), warnings };
                    }
                }

                ecs::Registry::createComponent(typeId, ent, buffer.data());
            }

            s_view.end_read(); // Components
            s_view.end_read(); // Object

            new(target) ecs::entity_data(*ent.data);

            return { common::success, warnings };
        }
    }

    common::result<void, fs_error> serializer<ecs::entity_data>::serialize(const void* ent, serializer_view& s_view, std::string_view name)
    {
        return detail::serialize_ent_data(*static_cast<const ecs::entity_data*>(ent), s_view, name);
    }

    common::result<void, fs_error> serializer<ecs::entity_data>::deserialize(void* target, serializer_view& view, std::string_view name)
    {
        return detail::deserialize_ent_data(target, view, name);
    }

    common::result<void, fs_error> serializer<ecs::entity>::serialize(const void* ent, serializer_view& s_view, std::string_view name)
    {
        return detail::serialize_ent_data(*static_cast<const ecs::entity*>(ent)->data, s_view, name);
    }

    common::result<void, fs_error> serializer<ecs::entity>::deserialize(void* target, serializer_view& view, std::string_view name)
    {
        byte rawData[sizeof(ecs::entity_data)];
        ecs::entity_data* data = reinterpret_cast<ecs::entity_data*>(rawData);
        auto result = detail::deserialize_ent_data(data, view, name);
        if (!result.has_error())
            new(target) ecs::entity(ecs::Registry::getEntity(data->id));
        return result;
    }
}

#undef EndReadPropagate
#undef EndObjectPropagate
