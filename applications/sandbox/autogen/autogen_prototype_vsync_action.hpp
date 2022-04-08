#pragma once
#include <core/types/prototype.hpp>
struct vsync_action;
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<::vsync_action>(const ::vsync_action& obj);
}
