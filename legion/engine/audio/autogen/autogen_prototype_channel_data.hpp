#pragma once
#include <core/types/prototype.hpp>
namespace legion::audio::detail
{
    struct channel_data;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::audio::detail::channel_data>(const legion::audio::detail::channel_data& obj);
}
