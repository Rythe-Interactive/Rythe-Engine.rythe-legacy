#pragma once
#include <core/types/prototype.hpp>
namespace legion::core
{
    struct mesh_filter;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::mesh_filter>(const legion::core::mesh_filter& obj);
}
