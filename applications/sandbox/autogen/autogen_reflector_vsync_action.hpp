#pragma once
#include <core/types/reflector.hpp>
struct vsync_action;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::vsync_action>(::vsync_action& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::vsync_action>(const ::vsync_action& obj);
}
