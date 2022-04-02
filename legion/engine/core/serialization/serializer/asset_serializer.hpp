#pragma once
#include <core/assets/assets.hpp>
#include <core/serialization/serializer/serializer_base.hpp>

namespace legion::core::serialization
{
    template<typename AssetType>
    struct serializer<assets::asset<AssetType>> : serializer_base
    {
        NO_DTOR_RULE5_NOEXCEPT(serializer);
        virtual ~serializer() = default;

        virtual common::result<void, fs_error> serialize(const void* src, serializer_view& view, std::string_view name) override;
        virtual common::result<void, fs_error> deserialize(void* target, serializer_view& view, std::string_view name) override;
        virtual id_type type_size() override { return sizeof(assets::asset<AssetType>); }
    };
}
