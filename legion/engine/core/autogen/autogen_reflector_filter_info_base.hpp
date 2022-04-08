#pragma once
#include <core/types/reflector.hpp>
namespace legion::core::ecs
{
    struct filter_info_base;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::ecs::filter_info_base>(legion::core::ecs::filter_info_base& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::ecs::filter_info_base>(const legion::core::ecs::filter_info_base& obj);
}
