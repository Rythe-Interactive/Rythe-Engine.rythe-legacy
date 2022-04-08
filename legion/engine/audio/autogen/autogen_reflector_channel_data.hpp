#pragma once
#include <core/types/reflector.hpp>
namespace legion::audio::detail
{
    struct channel_data;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::audio::detail::channel_data>(legion::audio::detail::channel_data& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::audio::detail::channel_data>(const legion::audio::detail::channel_data& obj);
}
