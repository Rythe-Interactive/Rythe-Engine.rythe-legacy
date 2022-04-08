#pragma once
#include <core/types/prototype.hpp>
struct player_move;
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<::player_move>(const ::player_move& obj);
}
