#pragma once
#include <core/types/prototype.hpp>
namespace legion::audio
{
    struct audio_segment_handle;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::audio::audio_segment_handle>(const legion::audio::audio_segment_handle& obj);
}
