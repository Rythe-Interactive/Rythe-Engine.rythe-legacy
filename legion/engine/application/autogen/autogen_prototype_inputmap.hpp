#pragma once
#include <core/types/prototype.hpp>
namespace legion::application
{
    struct inputmap;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::application::inputmap>(const legion::application::inputmap& obj);
}
