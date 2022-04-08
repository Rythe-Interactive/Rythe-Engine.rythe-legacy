#pragma once
#include <core/types/reflector.hpp>
namespace legion::core::filesystem::detail
{
    struct resource_converter_base;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::filesystem::detail::resource_converter_base>(legion::core::filesystem::detail::resource_converter_base& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::filesystem::detail::resource_converter_base>(const legion::core::filesystem::detail::resource_converter_base& obj);
}
