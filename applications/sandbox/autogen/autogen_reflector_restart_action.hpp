#pragma once
#include <core/types/reflector.hpp>
struct restart_action;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::restart_action>(::restart_action& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::restart_action>(const ::restart_action& obj);
}
