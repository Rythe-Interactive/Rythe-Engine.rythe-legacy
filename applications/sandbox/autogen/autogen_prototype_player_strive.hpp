#pragma once
#include <core/types/prototype.hpp>
struct player_strive;
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<::player_strive>(const ::player_strive& obj);
}
