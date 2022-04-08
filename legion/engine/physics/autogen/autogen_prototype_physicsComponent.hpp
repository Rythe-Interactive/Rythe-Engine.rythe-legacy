#pragma once
#include <core/types/prototype.hpp>
namespace legion::physics
{
    struct physicsComponent;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::physics::physicsComponent>(const legion::physics::physicsComponent& obj);
}
