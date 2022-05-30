#pragma once
#include <cmath>
#include <limits>

#include <core/platform/platform.hpp>
#include <core/types/primitives.hpp>
#include <core/math/util/meta.hpp>

namespace legion::core::math
{
    template<typename Scalar>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto round(Scalar val) noexcept;

    template<typename Integer = int, typename Scalar = float>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto iround(Scalar val) noexcept;

    template<typename Scalar>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto uround(Scalar val) noexcept;

    template<typename Scalar>
    L_NODISCARD L_ALWAYS_INLINE constexpr Scalar ceil(Scalar val) noexcept;
}

#include <core/math/basic/round.inl>
