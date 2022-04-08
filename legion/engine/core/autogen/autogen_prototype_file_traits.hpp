#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::filesystem
{
    struct file_traits;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::filesystem::file_traits>(const legion::core::filesystem::file_traits& obj);
}
