#pragma once
#include <core/types/prototype.hpp>
namespace legion::core
{
    struct example_comp;
}
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::example_comp>(const legion::core::example_comp& obj);
}
