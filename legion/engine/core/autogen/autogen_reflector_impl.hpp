#pragma once
#include <core/types/reflector.hpp>
namespace legion::core::log
{
    struct impl;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::log::impl>(legion::core::log::impl& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::log::impl>(const legion::core::log::impl& obj);
}
