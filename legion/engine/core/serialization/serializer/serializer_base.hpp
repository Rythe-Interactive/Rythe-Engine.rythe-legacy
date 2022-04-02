#pragma once
#include <core/types/meta.hpp>
#include <core/types/reflector.hpp>
#include <core/types/prototype.hpp>
#include <core/serialization/serializer_views/serializer_views.hpp>

namespace legion::core::serialization
{
    struct serializer_base
    {
        NO_DTOR_RULE5_NOEXCEPT(serializer_base);
        virtual ~serializer_base() = default;

        virtual common::result<void, fs_error> serialize(const void* serializable, serializer_view& view, std::string_view name) LEGION_PURE;
        virtual common::result<void, fs_error> deserialize(void* target, serializer_view& view, std::string_view name) LEGION_PURE;
        virtual id_type type_size() LEGION_PURE;
    };

    template<typename serializable_type>
    struct serializer : serializer_base
    {
        NO_DTOR_RULE5_NOEXCEPT(serializer);
        virtual ~serializer() = default;

        virtual common::result<void, fs_error> serialize(const void* serializable, serializer_view& s_view, std::string_view name) override;
        virtual common::result<void, fs_error> deserialize(void* target, serializer_view& s_view, std::string_view name) override;
        virtual id_type type_size() override { return sizeof(serializable_type); }
    };
}





