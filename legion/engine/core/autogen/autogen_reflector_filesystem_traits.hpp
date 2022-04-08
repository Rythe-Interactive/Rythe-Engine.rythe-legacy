#pragma once
#include <core/types/reflector.hpp>
namespace legion::core::filesystem
{
    struct filesystem_traits;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::filesystem::filesystem_traits>(legion::core::filesystem::filesystem_traits& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::filesystem::filesystem_traits>(const legion::core::filesystem::filesystem_traits& obj);
}
