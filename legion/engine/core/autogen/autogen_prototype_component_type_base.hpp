#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::ecs
{
    struct component_type_base;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::ecs::component_type_base>(const legion::core::ecs::component_type_base& obj);
}
