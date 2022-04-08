#pragma once
#include <core/types/reflector.hpp>
struct reload_shaders_action;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::reload_shaders_action>(::reload_shaders_action& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::reload_shaders_action>(const ::reload_shaders_action& obj);
}
