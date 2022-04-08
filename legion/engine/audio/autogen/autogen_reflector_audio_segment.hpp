#pragma once
#include <core/types/reflector.hpp>
namespace legion::audio
{
    struct audio_segment;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::audio::audio_segment>(legion::audio::audio_segment& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::audio::audio_segment>(const legion::audio::audio_segment& obj);
}
