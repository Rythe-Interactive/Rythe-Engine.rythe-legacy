#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::ecs
{
    struct component_pool_base;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::ecs::component_pool_base>(const legion::core::ecs::component_pool_base& obj);
}
