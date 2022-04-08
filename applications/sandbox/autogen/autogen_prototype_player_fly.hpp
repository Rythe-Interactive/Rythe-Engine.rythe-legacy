#pragma once
#include <core/types/prototype.hpp>
struct player_fly;
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<::player_fly>(const ::player_fly& obj);
}
