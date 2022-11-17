#include <core/math/geometric/normalize.hpp>
#pragma once

#include <core/math/geometric/normalize_vector.inl>
#include <core/math/geometric/normalize_quaternion.inl>

namespace legion::core::math
{
    template<typename vec_type, ::std::enable_if_t<is_vector_v<vec_type>, bool>>
    L_NODISCARD L_ALWAYS_INLINE auto normalize(const vec_type& v) noexcept
    {
        return detail::compute_normalize<make_vector_t<vec_type>>::compute(v);
    }

    template<typename quat_type, ::std::enable_if_t<is_quat_v<quat_type>, bool>>
    L_NODISCARD L_ALWAYS_INLINE auto normalize(const quat_type& v) noexcept
    {
        return detail::compute_normalize<make_quat_t<quat_type>>::compute(v);
    }
}
