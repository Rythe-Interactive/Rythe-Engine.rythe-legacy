#pragma once
#include <core/types/primitives.hpp>
#include <core/math/matrix/matrix_base.hpp>
#include <core/math/meta.hpp>

namespace legion::core::math
{
    template<typename mat_type, ::std::enable_if_t<is_matrix_v<mat_type>, bool> = true>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto determinant(const mat_type& mat) noexcept;
}

#include <core/math/matrix/determinant.inl>
