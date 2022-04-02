#pragma once
#include <core/ecs/ecs.hpp>
#include <core/serialization/serializer/serializer_base.hpp>

namespace legion::core::serialization
{
    template<>
    struct serializer<ecs::entity_data> : serializer_base
    {
        NO_DTOR_RULE5_NOEXCEPT(serializer);
        virtual ~serializer() = default;

        virtual common::result<void, fs_error> serialize(const void* serializable, serializer_view& view, std::string_view name) override;
        virtual common::result<void, fs_error> deserialize(void* target, serializer_view& s_view, std::string_view name) override;
        virtual id_type type_size() override { return sizeof(ecs::entity_data); }
    };

    template<>
    struct serializer<ecs::entity> : serializer_base
    {
        NO_DTOR_RULE5_NOEXCEPT(serializer);
        virtual ~serializer() = default;

        virtual common::result<void, fs_error> serialize(const void* serializable, serializer_view& view, std::string_view name) override;
        virtual common::result<void, fs_error> deserialize(void* target, serializer_view& s_view, std::string_view name) override;
        virtual id_type type_size() override { return sizeof(ecs::entity); }
    };
}
