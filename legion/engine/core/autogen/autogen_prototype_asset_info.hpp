#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::assets::detail
{
    struct asset_info;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::assets::detail::asset_info>(const legion::core::assets::detail::asset_info& obj);
}
