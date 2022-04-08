#pragma once
#include <core/types/prototype.hpp>
namespace legion::core
{
    struct type_hash;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::type_hash>(const legion::core::type_hash& obj);
}
