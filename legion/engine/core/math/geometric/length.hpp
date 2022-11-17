#pragma once
#include <cmath>

#include <core/types/primitives.hpp>
#include <core/math/vector/vector.hpp>
#include <core/math/geometric/dot.hpp>
#include <core/math/matrix/determinant.hpp>
#include <core/math/quaternion/quaternion.hpp>

namespace legion::core::math
{
    template<typename vec_type, ::std::enable_if_t<is_vector_v<vec_type>, bool> = true>
    L_NODISCARD L_ALWAYS_INLINE auto length(const vec_type& v) noexcept;

    template<typename vec_type, ::std::enable_if_t<is_vector_v<vec_type>, bool> = true>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto length2(const vec_type& v) noexcept;

    template<typename quat_type, ::std::enable_if_t<is_quat_v<quat_type>, bool> = true>
    L_NODISCARD L_ALWAYS_INLINE auto length(const quat_type& q) noexcept;

    template<typename quat_type, ::std::enable_if_t<is_quat_v<quat_type>, bool> = true>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto length2(const quat_type& q) noexcept;

    template<typename mat_type, ::std::enable_if_t<is_matrix_v<mat_type>, bool> = true>
    L_NODISCARD L_ALWAYS_INLINE auto length(const mat_type& m) noexcept;

    template<typename mat_type, ::std::enable_if_t<is_matrix_v<mat_type>, bool> = true>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto length2(const mat_type& m) noexcept;
}

#include <core/math/geometric/length.inl>
