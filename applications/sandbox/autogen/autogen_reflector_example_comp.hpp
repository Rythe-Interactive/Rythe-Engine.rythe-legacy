#pragma once
#include <core/types/reflector.hpp>
struct example_comp;
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<::example_comp>(::example_comp& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const ::example_comp>(const ::example_comp& obj);
}
