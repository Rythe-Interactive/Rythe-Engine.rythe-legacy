#pragma once
#include <core/types/prototype.hpp>
namespace legion::core
{
    struct position;
}
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::position>(const legion::core::position& obj);
}
