#pragma once
#include <core/types/prototype.hpp>
namespace legion::physics
{
    struct ConvexConvexCollisionInfo;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::physics::ConvexConvexCollisionInfo>(const legion::physics::ConvexConvexCollisionInfo& obj);
}
