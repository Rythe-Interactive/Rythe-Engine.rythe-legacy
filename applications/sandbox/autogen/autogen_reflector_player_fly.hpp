#pragma once
#include <core/types/reflector.hpp>
struct player_fly;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::player_fly>(::player_fly& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::player_fly>(const ::player_fly& obj);
}
