#pragma once
#include <core/types/reflector.hpp>
struct AddRigidbody;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::AddRigidbody>(::AddRigidbody& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::AddRigidbody>(const ::AddRigidbody& obj);
}
