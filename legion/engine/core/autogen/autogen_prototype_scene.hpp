#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::scenemanagement
{
    struct scene;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::scenemanagement::scene>(const legion::core::scenemanagement::scene& obj);
}
