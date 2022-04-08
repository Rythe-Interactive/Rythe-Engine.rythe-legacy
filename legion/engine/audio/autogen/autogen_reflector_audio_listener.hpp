#pragma once
#include <core/types/reflector.hpp>
namespace legion::audio
{
    struct audio_listener;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::audio::audio_listener>(legion::audio::audio_listener& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::audio::audio_listener>(const legion::audio::audio_listener& obj);
}
