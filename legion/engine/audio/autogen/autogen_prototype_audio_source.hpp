#pragma once
#include <core/types/prototype.hpp>
namespace legion::audio
{
    struct audio_source;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::audio::audio_source>(const legion::audio::audio_source& obj);
}
