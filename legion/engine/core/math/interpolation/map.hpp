#pragma once
#include <core/types/primitives.hpp>
#include <core/math/vector/vector.hpp>
#include <core/math/matrix/matrix.hpp>
#include <core/math/quaternion/quaternion_base.hpp>
#include <core/math/util/meta.hpp>
#include <core/math/basic/constraint.hpp>

namespace legion::core::math
{
    // map
    template<typename T, typename InMin, typename InMax, typename OutMin, typename OutMax>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto map(T&& value, InMin&& inMin, InMax&& inMax, OutMin&& outMin, OutMax&& outMax) noexcept;

    template<typename T, typename InMin, typename InMax>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto map01(T&& value, InMin&& inMin, InMax&& inMax) noexcept;
}

#include <core/math/interpolation/map.inl>
