#include <core/serialization/serializer/entity_serializer.hpp>
#pragma once
#include <core/serialization/util/entity_util.hpp>

namespace legion::core::serialization
{
    inline common::result<void, fs_error> serializer<ecs::entity_data>::serialize(const void* ent, serializer_view& s_view, std::string_view name)
    {
        return util::serialize_ent_data(*static_cast<const ecs::entity_data*>(ent), s_view, name);
    }

    inline common::result<void, fs_error> serializer<ecs::entity_data>::deserialize(void* target, serializer_view& view, std::string_view name)
    {
        return util::deserialize_ent_data(reinterpret_cast<ecs::entity_data*>(target), view, name);
    }

    inline common::result<void, fs_error> serializer<ecs::entity>::serialize(const void* ent, serializer_view& s_view, std::string_view name)
    {
        return util::serialize_ent_data(*static_cast<const ecs::entity*>(ent)->data, s_view, name);
    }

    inline common::result<void, fs_error> serializer<ecs::entity>::deserialize(void* target, serializer_view& view, std::string_view name)
    {
        byte rawData[sizeof(ecs::entity_data)];
        ecs::entity_data* data = reinterpret_cast<ecs::entity_data*>(rawData);
        auto result = util::deserialize_ent_data(data, view, name);
        *static_cast<ecs::entity*>(target) = ecs::Registry::getEntity(data->id);
        return result;
    }
}
