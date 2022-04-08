#pragma once
#include <core/types/reflector.hpp>
namespace legion::application
{
    struct inputmap;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::application::inputmap>(legion::application::inputmap& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::application::inputmap>(const legion::application::inputmap& obj);
}
