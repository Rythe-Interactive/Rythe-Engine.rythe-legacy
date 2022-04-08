#pragma once
#include <core/types/reflector.hpp>
struct player_strive;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::player_strive>(::player_strive& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::player_strive>(const ::player_strive& obj);
}
