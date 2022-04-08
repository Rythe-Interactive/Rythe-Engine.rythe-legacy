#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::serialization
{
    struct yaml;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::serialization::yaml>(const legion::core::serialization::yaml& obj);
}
