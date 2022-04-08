#pragma once
#include <core/types/prototype.hpp>
namespace legion::core
{
    struct reflectable_attribute;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::reflectable_attribute>(const legion::core::reflectable_attribute& obj);
}
