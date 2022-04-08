#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::serialization
{
    struct serializer_base;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::serialization::serializer_base>(const legion::core::serialization::serializer_base& obj);
}
