#pragma once
#include <core/types/reflector.hpp>
namespace legion::physics
{
    struct cube_collider_params;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::physics::cube_collider_params>(legion::physics::cube_collider_params& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::physics::cube_collider_params>(const legion::physics::cube_collider_params& obj);
}
