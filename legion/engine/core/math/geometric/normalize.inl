#include <core/math/geometric/normalize.hpp>
#pragma once

#include <core/math/geometric/normalize_vector.inl>

namespace legion::core::math
{
    template<typename vec_type, ::std::enable_if_t<is_vector_v<vec_type>, bool>>
    L_NODISCARD L_ALWAYS_INLINE auto normalize(const vec_type& v) noexcept
    {
        return detail::compute_normalize<vec_type>::compute(v);
    }
}
