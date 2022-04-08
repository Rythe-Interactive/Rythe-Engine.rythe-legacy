#pragma once
#include <core/types/reflector.hpp>
namespace legion::rendering
{
    struct texture_import_settings;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::rendering::texture_import_settings>(legion::rendering::texture_import_settings& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::rendering::texture_import_settings>(const legion::rendering::texture_import_settings& obj);
}
