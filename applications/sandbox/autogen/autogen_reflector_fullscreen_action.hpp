#pragma once
#include <core/types/reflector.hpp>
struct fullscreen_action;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::fullscreen_action>(::fullscreen_action& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::fullscreen_action>(const ::fullscreen_action& obj);
}
