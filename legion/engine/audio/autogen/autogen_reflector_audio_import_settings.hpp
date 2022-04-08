#pragma once
#include <core/types/reflector.hpp>
namespace legion::audio
{
    struct audio_import_settings;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::audio::audio_import_settings>(legion::audio::audio_import_settings& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::audio::audio_import_settings>(const legion::audio::audio_import_settings& obj);
}
