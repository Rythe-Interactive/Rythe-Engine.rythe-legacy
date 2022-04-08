#pragma once
#include <core/types/reflector.hpp>
namespace legion::core::ecs
{
    struct const_child_reverse_iterator;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::ecs::const_child_reverse_iterator>(legion::core::ecs::const_child_reverse_iterator& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::ecs::const_child_reverse_iterator>(const legion::core::ecs::const_child_reverse_iterator& obj);
}
