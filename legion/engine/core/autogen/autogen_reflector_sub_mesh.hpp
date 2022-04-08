#pragma once
#include <core/types/reflector.hpp>
namespace legion::core
{
    struct sub_mesh;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::sub_mesh>(legion::core::sub_mesh& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::sub_mesh>(const legion::core::sub_mesh& obj);
}
