#pragma once
#include <core/types/reflector.hpp>
struct switch_skybox_action;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::switch_skybox_action>(::switch_skybox_action& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::switch_skybox_action>(const ::switch_skybox_action& obj);
}
