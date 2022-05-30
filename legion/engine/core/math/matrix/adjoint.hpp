#pragma once
#include <cmath>

#include <core/math/matrix/determinant.hpp>

namespace legion::core::math
{
    template<typename mat_type, ::std::enable_if_t<is_matrix_v<mat_type>, bool> = true>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto adjoint(const mat_type& mat) noexcept;
}

#include <core/math/matrix/adjoint.inl>
