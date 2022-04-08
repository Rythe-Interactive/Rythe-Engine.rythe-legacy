#pragma once
#include <core/types/reflector.hpp>
namespace legion::core
{
    struct mesh_filter;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::mesh_filter>(legion::core::mesh_filter& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::mesh_filter>(const legion::core::mesh_filter& obj);
}
