#pragma once
#include <core/types/reflector.hpp>
struct gladGLversionStruct;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::gladGLversionStruct>(::gladGLversionStruct& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::gladGLversionStruct>(const ::gladGLversionStruct& obj);
}
