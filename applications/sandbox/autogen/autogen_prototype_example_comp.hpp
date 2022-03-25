#pragma once
#include <core/types/prototype.hpp>
struct example_comp;
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::example_comp>(const ::example_comp& obj);
}
