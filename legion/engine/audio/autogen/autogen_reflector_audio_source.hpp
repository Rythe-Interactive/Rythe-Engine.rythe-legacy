#pragma once
#include <core/types/reflector.hpp>
namespace legion::audio
{
    struct audio_source;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::audio::audio_source>(legion::audio::audio_source& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::audio::audio_source>(const legion::audio::audio_source& obj);
}
