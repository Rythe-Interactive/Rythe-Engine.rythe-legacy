#pragma once
#include <core/types/reflector.hpp>
struct auto_exposure_action;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::auto_exposure_action>(::auto_exposure_action& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::auto_exposure_action>(const ::auto_exposure_action& obj);
}
