#pragma once
#include <core/types/prototype.hpp>
struct reload_shaders_action;
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<::reload_shaders_action>(const ::reload_shaders_action& obj);
}
