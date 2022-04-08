#pragma once
#include <core/types/prototype.hpp>
namespace legion::core
{
    struct this_engine;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::this_engine>(const legion::core::this_engine& obj);
}
