#pragma once
#include <core/types/prototype.hpp>
namespace legion::core
{
    struct mesh;
}
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::mesh>(const legion::core::mesh& obj);
}
