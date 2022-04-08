#pragma once
#include <core/types/reflector.hpp>
namespace legion::core::scenemanagement
{
    struct scene;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::scenemanagement::scene>(legion::core::scenemanagement::scene& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::scenemanagement::scene>(const legion::core::scenemanagement::scene& obj);
}
