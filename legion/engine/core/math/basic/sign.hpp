#pragma once
#include <cmath>
#include <limits>

#include <core/platform/platform.hpp>
#include <core/types/primitives.hpp>
#include <core/math/util/meta.hpp>

namespace legion::core::math
{
    template<typename Scalar>
    L_NODISCARD L_ALWAYS_INLINE constexpr Scalar sign(Scalar val) noexcept
    {
        if constexpr (::std::is_signed_v<Scalar>)
            return static_cast<Scalar>(1);
        else
            return (static_cast<Scalar>(0) <= val) - (val < static_cast<Scalar>(0));
    }

    template<typename Scalar>
    L_NODISCARD L_ALWAYS_INLINE constexpr Scalar sign(Scalar val) noexcept

    template<typename Scalar>
    L_NODISCARD L_ALWAYS_INLINE constexpr Scalar signum(Scalar val) noexcept
    {
        if constexpr (::std::is_signed_v<Scalar>)
            return static_cast<Scalar>(0) < val;
        else
            return (static_cast<Scalar>(0) < val) - (val < static_cast<Scalar>(0));
    }
}
