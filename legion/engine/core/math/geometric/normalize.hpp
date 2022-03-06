#pragma once
#include <cmath>

#include <core/types/primitives.hpp>
#include <core/math/vector/vector.hpp>
#include <core/math/arithmetic/division.hpp>
#include <core/math/geometric/length.hpp>

namespace legion::core::math
{
    template<typename vec_type, ::std::enable_if_t<is_vector_v<vec_type>, bool> = true>
    L_NODISCARD L_ALWAYS_INLINE auto normalize(const vec_type& v) noexcept;
}

#include <core/math/geometric/normalize.inl>
