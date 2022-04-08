#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::log
{
    struct impl;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::log::impl>(const legion::core::log::impl& obj);
}
