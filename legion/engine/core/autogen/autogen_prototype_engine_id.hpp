#pragma once
#include <core/types/prototype.hpp>
namespace legion::core
{
    struct engine_id;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::engine_id>(const legion::core::engine_id& obj);
}
