#pragma once
#include <core/types/reflector.hpp>
namespace legion::core::assets::detail
{
    struct asset_info;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::assets::detail::asset_info>(legion::core::assets::detail::asset_info& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::assets::detail::asset_info>(const legion::core::assets::detail::asset_info& obj);
}
