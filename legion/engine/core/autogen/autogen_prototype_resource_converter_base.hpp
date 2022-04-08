#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::filesystem::detail
{
    struct resource_converter_base;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::filesystem::detail::resource_converter_base>(const legion::core::filesystem::detail::resource_converter_base& obj);
}
