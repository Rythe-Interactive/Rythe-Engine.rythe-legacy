#pragma once
#include <core/types/reflector.hpp>
struct player_speedup;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::player_speedup>(::player_speedup& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::player_speedup>(const ::player_speedup& obj);
}
