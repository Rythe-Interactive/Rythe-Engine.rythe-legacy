#pragma once
#include <core/types/prototype.hpp>
namespace legion::core
{
    struct name_hash;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::name_hash>(const legion::core::name_hash& obj);
}
