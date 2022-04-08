#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::ecs
{
    struct filter_info_base;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::ecs::filter_info_base>(const legion::core::ecs::filter_info_base& obj);
}
