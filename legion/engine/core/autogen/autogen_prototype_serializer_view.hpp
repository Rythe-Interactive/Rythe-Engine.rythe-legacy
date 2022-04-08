#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::serialization
{
    struct serializer_view;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::serialization::serializer_view>(const legion::core::serialization::serializer_view& obj);
}
