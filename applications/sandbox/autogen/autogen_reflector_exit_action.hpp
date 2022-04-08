#pragma once
#include <core/types/reflector.hpp>
struct exit_action;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::exit_action>(::exit_action& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::exit_action>(const ::exit_action& obj);
}
