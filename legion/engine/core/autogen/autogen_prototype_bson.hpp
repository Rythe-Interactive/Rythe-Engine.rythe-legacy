#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::serialization
{
    struct bson;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::serialization::bson>(const legion::core::serialization::bson& obj);
}
