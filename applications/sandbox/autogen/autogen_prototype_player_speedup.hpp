#pragma once
#include <core/types/prototype.hpp>
struct player_speedup;
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<::player_speedup>(const ::player_speedup& obj);
}
