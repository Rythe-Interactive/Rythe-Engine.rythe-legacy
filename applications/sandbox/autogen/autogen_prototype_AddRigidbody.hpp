#pragma once
#include <core/types/prototype.hpp>
struct AddRigidbody;
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<::AddRigidbody>(const ::AddRigidbody& obj);
}
