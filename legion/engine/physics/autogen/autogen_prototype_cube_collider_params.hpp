#pragma once
#include <core/types/prototype.hpp>
namespace legion::physics
{
    struct cube_collider_params;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::physics::cube_collider_params>(const legion::physics::cube_collider_params& obj);
}
