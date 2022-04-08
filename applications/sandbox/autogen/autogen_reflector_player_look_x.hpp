#pragma once
#include <core/types/reflector.hpp>
struct player_look_x;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::player_look_x>(::player_look_x& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::player_look_x>(const ::player_look_x& obj);
}
