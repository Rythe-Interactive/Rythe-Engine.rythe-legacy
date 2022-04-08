#pragma once
#include <core/types/reflector.hpp>
struct tonemap_action;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::tonemap_action>(::tonemap_action& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::tonemap_action>(const ::tonemap_action& obj);
}
