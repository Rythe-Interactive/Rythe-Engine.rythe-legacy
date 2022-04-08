#pragma once
#include <core/types/prototype.hpp>
namespace legion::rendering
{
    struct model_handle;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::rendering::model_handle>(const legion::rendering::model_handle& obj);
}
