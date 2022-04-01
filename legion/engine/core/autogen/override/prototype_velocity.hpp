#pragma once
#include <core/types/prototype.hpp>
namespace legion::core
{
    struct velocity;
}
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::velocity>(const legion::core::velocity& obj);
}
