#pragma once
#include <core/types/primitives.hpp>
#include <core/math/vector/vector.hpp>
#include <core/math/matrix/matrix.hpp>
#include <core/math/quaternion/quaternion_base.hpp>
#include <core/math/util/meta.hpp>
#include <core/math/basic/constraint.hpp>

namespace legion::core::math
{
    // mul
    template<typename TypeA, typename TypeB>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto mul(TypeA&& a, TypeB&& b) noexcept;

    // operator *
    template<typename TypeA, typename TypeB, std::enable_if_t<is_linear_algebraic_construct_v<TypeA> || is_linear_algebraic_construct_v<TypeB>, bool> = true>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto operator*(TypeA&& a, TypeB&& b) noexcept;

    // mul_assign
    template<typename TypeA, typename TypeB>
    constexpr TypeA& mul_assign(TypeA& a, TypeB&& b) noexcept;

    // operator *=
    template<typename TypeA, typename TypeB, std::enable_if_t<is_linear_algebraic_construct_v<TypeA> || is_linear_algebraic_construct_v<TypeB>, bool> = true>
    constexpr TypeA& operator*=(TypeA& a, TypeB&& b) noexcept;
}

#include <core/math/arithmetic/multiplication.inl>
