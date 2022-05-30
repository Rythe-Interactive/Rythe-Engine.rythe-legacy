#pragma once
#include <core/types/primitives.hpp>
#include <core/math/vector/vector.hpp>
#include <core/math/arithmetic/division.hpp>
#include <core/math/arithmetic/negate.hpp>
#include <core/math/quaternion/quaternion.hpp>
#include <core/math/matrix/matrix.hpp>

namespace legion::core::math
{
    template<typename vec_type, ::std::enable_if_t<is_vector_v<vec_type>, bool> = true>
    L_NODISCARD L_ALWAYS_INLINE auto inverse(const vec_type& v) noexcept;

    template<typename quat_type, ::std::enable_if_t<is_quat_v<quat_type>, bool> = true>
    L_NODISCARD L_ALWAYS_INLINE auto inverse(const quat_type& q) noexcept;

    template<typename mat_type, ::std::enable_if_t<is_matrix_v<mat_type>, bool> = true>
    L_NODISCARD L_ALWAYS_INLINE auto inverse(const mat_type& m) noexcept;
}

#include <core/math/geometric/inverse.inl>
