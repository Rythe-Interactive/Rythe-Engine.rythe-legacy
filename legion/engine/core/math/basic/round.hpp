#pragma once
#include <cmath>
#include <limits>

#include <core/platform/platform.hpp>
#include <core/types/primitives.hpp>
#include <core/math/util/meta.hpp>
#include <core/math/basic/sign.hpp>

namespace legion::core::math
{
    enum struct round_mode
    {
        floor, round, ceil, trunc
    };

    template<typename Target, round_mode Mode = round_mode::round, typename T>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto adv_round(T&& val) noexcept;

    template<typename T>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto round(T&& val) noexcept;

    template<typename Integer = int, typename T>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto iround(T&& val) noexcept;

    template<typename T>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto uround(T&& val) noexcept;

    template<typename T>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto ceil(T&& val) noexcept;

    template<typename Integer = int, typename T>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto iceil(T&& val) noexcept;

    template<typename T>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto uceil(T&& val) noexcept;

    template<typename T>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto floor(T&& val) noexcept;

    template<typename Integer = int, typename T>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto ifloor(T&& val) noexcept;

    template<typename T>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto ufloor(T&& val) noexcept;

    template<typename T>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto trunc(T&& val) noexcept;

    template<typename Integer = int, typename T>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto itrunc(T&& val) noexcept;

    template<typename T>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto utrunc(T&& val) noexcept;
}

#include <core/math/basic/round.inl>
