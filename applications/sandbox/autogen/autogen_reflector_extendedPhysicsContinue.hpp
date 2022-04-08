#pragma once
#include <core/types/reflector.hpp>
struct extendedPhysicsContinue;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::extendedPhysicsContinue>(::extendedPhysicsContinue& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::extendedPhysicsContinue>(const ::extendedPhysicsContinue& obj);
}
