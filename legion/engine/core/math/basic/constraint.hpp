#pragma once
#include <core/platform/platform.hpp>

#include <core/math/vector/vector.hpp>
#include <core/math/util/meta.hpp>

namespace legion::core::math
{
    template<typename A, typename B>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto min(A&& a, B&& b);

    template<typename A, typename B>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto max(A&& a, B&& b);

    template<typename In, typename Min, typename Max>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto clamp(In&& in, Min&& min, Max&& max);

    template<typename In>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto saturate(In&& in);
}

#include <core/math/basic/constraint.inl>
