#pragma once
#include <core/types/reflector.hpp>
namespace legion::physics
{
    struct ConvexConvexCollisionInfo;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::physics::ConvexConvexCollisionInfo>(legion::physics::ConvexConvexCollisionInfo& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::physics::ConvexConvexCollisionInfo>(const legion::physics::ConvexConvexCollisionInfo& obj);
}
