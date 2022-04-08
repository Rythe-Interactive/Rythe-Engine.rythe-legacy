#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::serialization
{
    struct json;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::serialization::json>(const legion::core::serialization::json& obj);
}
