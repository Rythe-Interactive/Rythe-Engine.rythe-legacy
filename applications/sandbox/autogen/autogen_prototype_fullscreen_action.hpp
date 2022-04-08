#pragma once
#include <core/types/prototype.hpp>
struct fullscreen_action;
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<::fullscreen_action>(const ::fullscreen_action& obj);
}
