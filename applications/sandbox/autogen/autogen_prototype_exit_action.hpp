#pragma once
#include <core/types/prototype.hpp>
struct exit_action;
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<::exit_action>(const ::exit_action& obj);
}
