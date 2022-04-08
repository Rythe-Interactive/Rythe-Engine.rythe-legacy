#pragma once
#include <core/types/prototype.hpp>
namespace legion::core::filesystem
{
    struct filesystem_traits;
}
namespace legion::core
{
    template<>
    L_NODISCARD extern prototype make_prototype<legion::core::filesystem::filesystem_traits>(const legion::core::filesystem::filesystem_traits& obj);
}
