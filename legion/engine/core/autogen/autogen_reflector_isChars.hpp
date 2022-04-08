#pragma once
#include <core/types/reflector.hpp>
namespace legion::core::common
{
    struct isChars;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::common::isChars>(legion::core::common::isChars& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::common::isChars>(const legion::core::common::isChars& obj);
}
