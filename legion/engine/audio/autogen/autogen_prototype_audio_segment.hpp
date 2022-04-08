#pragma once
#include <core/types/prototype.hpp>
namespace legion::audio
{
    struct audio_segment;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::audio::audio_segment>(const legion::audio::audio_segment& obj);
}
