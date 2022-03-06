#include <core/math/geometric/inverse.hpp>
#pragma once

#include <core/math/geometric/inverse_vector.inl>
#include <core/math/geometric/inverse_quat.inl>

namespace legion::core::math
{
    template<typename vec_type, ::std::enable_if_t<is_vector_v<vec_type>, bool>>
    L_NODISCARD L_ALWAYS_INLINE auto inverse(const vec_type& v) noexcept
    {
        return detail::compute_inverse<vec_type>::compute(v);
    }

    template<typename quat_type, ::std::enable_if_t<is_quat_v<quat_type>, bool>>
    L_NODISCARD L_ALWAYS_INLINE auto inverse(const quat_type& q) noexcept
    {
        return detail::compute_inverse<quat_type>::compute(q);
    }

    template<typename mat_type, ::std::enable_if_t<is_matrix_v<mat_type>, bool>>
    L_NODISCARD L_ALWAYS_INLINE auto inverse(const mat_type& m) noexcept
    {
        return detail::compute_inverse<mat_type>::compute(m);
    }
}
