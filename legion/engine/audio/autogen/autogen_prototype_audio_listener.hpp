#pragma once
#include <core/types/prototype.hpp>
namespace legion::audio
{
    struct audio_listener;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::audio::audio_listener>(const legion::audio::audio_listener& obj);
}
