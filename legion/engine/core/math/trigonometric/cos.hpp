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
    // cos
    template<typename T>
    L_NODISCARD L_ALWAYS_INLINE auto cos(T&& v) noexcept;

    // acos
    template<typename T>
    L_NODISCARD L_ALWAYS_INLINE auto acos(T&& v) noexcept;
}

#include <core/math/trigonometric/cos.inl>
