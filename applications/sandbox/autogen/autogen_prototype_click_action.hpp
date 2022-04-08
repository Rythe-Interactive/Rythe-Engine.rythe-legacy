#pragma once
#include <core/types/prototype.hpp>
namespace legion
{
    struct click_action;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::click_action>(const legion::click_action& obj);
}
