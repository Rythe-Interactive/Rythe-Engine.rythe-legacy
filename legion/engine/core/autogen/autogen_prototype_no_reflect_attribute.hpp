#pragma once
#include <core/types/prototype.hpp>
namespace legion::core
{
    struct no_reflect_attribute;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::no_reflect_attribute>(const legion::core::no_reflect_attribute& obj);
}
