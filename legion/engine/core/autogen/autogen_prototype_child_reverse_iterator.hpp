#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::ecs
{
    struct child_reverse_iterator;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::ecs::child_reverse_iterator>(const legion::core::ecs::child_reverse_iterator& obj);
}
