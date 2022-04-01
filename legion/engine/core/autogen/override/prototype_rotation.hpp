#pragma once
#include <core/types/prototype.hpp>
namespace legion::core
{
    struct rotation;
}
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::rotation>(const legion::core::rotation& obj);
}
