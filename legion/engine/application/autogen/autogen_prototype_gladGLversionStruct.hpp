#pragma once
#include <core/types/prototype.hpp>
struct gladGLversionStruct;
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<::gladGLversionStruct>(const ::gladGLversionStruct& obj);
}
