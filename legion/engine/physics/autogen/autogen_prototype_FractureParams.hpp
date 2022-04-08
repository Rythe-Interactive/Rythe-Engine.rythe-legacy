#pragma once
#include <core/types/prototype.hpp>
namespace legion::physics
{
    struct FractureParams;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::physics::FractureParams>(const legion::physics::FractureParams& obj);
}
