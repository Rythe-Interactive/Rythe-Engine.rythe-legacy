#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::ecs
{
    struct const_child_iterator;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::ecs::const_child_iterator>(const legion::core::ecs::const_child_iterator& obj);
}
