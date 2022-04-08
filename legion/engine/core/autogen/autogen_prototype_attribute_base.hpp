#pragma once
#include <core/types/prototype.hpp>
namespace legion::core
{
    struct attribute_base;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::attribute_base>(const legion::core::attribute_base& obj);
}
