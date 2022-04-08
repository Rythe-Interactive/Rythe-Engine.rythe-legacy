#pragma once
#include <core/types/reflector.hpp>
namespace legion
{
    struct click_action;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::click_action>(legion::click_action& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::click_action>(const legion::click_action& obj);
}
