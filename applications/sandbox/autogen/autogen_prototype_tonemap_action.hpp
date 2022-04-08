#pragma once
#include <core/types/prototype.hpp>
struct tonemap_action;
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<::tonemap_action>(const ::tonemap_action& obj);
}
