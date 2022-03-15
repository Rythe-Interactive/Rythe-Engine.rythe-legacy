#pragma once
#include <core/types/prototype.hpp>
namespace legion::core
{
    struct scale;
}
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::scale>(const legion::core::scale& obj);
}
