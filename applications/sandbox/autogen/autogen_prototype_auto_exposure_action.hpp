#pragma once
#include <core/types/prototype.hpp>
struct auto_exposure_action;
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<::auto_exposure_action>(const ::auto_exposure_action& obj);
}
