#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::common
{
    struct isChars;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::common::isChars>(const legion::core::common::isChars& obj);
}
