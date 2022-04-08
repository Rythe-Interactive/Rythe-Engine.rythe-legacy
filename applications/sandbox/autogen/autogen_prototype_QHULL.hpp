#pragma once
#include <core/types/prototype.hpp>
struct QHULL;
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<::QHULL>(const ::QHULL& obj);
}
