#pragma once
#include <core/types/reflector.hpp>
namespace legion::physics
{
    struct FractureParams;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::physics::FractureParams>(legion::physics::FractureParams& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::physics::FractureParams>(const legion::physics::FractureParams& obj);
}
