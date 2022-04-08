#pragma once
#include <core/types/prototype.hpp>
struct restart_action;
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<::restart_action>(const ::restart_action& obj);
}
