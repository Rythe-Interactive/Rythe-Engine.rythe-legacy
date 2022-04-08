#pragma once
#include <core/types/reflector.hpp>
namespace legion::core
{
    struct engine_id;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::engine_id>(legion::core::engine_id& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::engine_id>(const legion::core::engine_id& obj);
}
