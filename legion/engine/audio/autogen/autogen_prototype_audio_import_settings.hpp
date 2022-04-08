#pragma once
#include <core/types/prototype.hpp>
namespace legion::audio
{
    struct audio_import_settings;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::audio::audio_import_settings>(const legion::audio::audio_import_settings& obj);
}
