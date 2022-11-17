#pragma once
#include <core/types/primitives.hpp>
#include <core/math/vector/vector.hpp>
#include <core/math/matrix/matrix.hpp>
#include <core/math/quaternion/quaternion_base.hpp>
#include <core/math/util/meta.hpp>
#include <core/math/basic/constraint.hpp>

#include <cmath>

namespace legion::core::math
{
    // sin
    template<typename T>
    L_NODISCARD L_ALWAYS_INLINE auto sin(T&& v) noexcept;

    // asin
    template<typename T>
    L_NODISCARD L_ALWAYS_INLINE auto asin(T&& v) noexcept;
}

#include <core/math/trigonometric/sin.inl>
