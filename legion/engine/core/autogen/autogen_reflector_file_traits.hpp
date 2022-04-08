#pragma once
#include <core/types/reflector.hpp>
namespace legion::core::filesystem
{
    struct file_traits;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern reflector make_reflector<legion::core::filesystem::file_traits>(legion::core::filesystem::file_traits& obj);
    template<>
    L_NODISCARD extern const reflector make_reflector<const legion::core::filesystem::file_traits>(const legion::core::filesystem::file_traits& obj);
}
