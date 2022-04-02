#pragma once
#include <core/types/prototype.hpp>
namespace legion::core
{
    struct sub_mesh;
}
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::sub_mesh>(const legion::core::sub_mesh& obj);
}
