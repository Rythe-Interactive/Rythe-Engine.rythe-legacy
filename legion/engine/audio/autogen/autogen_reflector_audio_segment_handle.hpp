#pragma once
#include <core/types/reflector.hpp>
namespace legion::audio
{
    struct audio_segment_handle;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::audio::audio_segment_handle>(legion::audio::audio_segment_handle& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::audio::audio_segment_handle>(const legion::audio::audio_segment_handle& obj);
}
