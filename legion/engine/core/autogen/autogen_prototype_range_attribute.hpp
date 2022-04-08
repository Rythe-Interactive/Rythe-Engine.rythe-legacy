#pragma once
#include <core/types/prototype.hpp>
namespace legion::core
{
    struct range_attribute;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::range_attribute>(const legion::core::range_attribute& obj);
}
