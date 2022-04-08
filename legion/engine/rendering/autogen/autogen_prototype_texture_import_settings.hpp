#pragma once
#include <core/types/prototype.hpp>
namespace legion::rendering
{
    struct texture_import_settings;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::rendering::texture_import_settings>(const legion::rendering::texture_import_settings& obj);
}
