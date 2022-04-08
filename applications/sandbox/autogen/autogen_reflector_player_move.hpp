#pragma once
#include <core/types/reflector.hpp>
struct player_move;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::player_move>(::player_move& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::player_move>(const ::player_move& obj);
}
