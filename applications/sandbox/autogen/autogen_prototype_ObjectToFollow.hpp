#pragma once
#include <core/types/prototype.hpp>
struct ObjectToFollow;
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<::ObjectToFollow>(const ::ObjectToFollow& obj);
}
