#pragma once
#include <core/types/primitives.hpp>
#include <core/math/vector/vector.hpp>
#include <core/math/matrix/matrix.hpp>
#include <core/math/quaternion/quaternion_base.hpp>
#include <core/math/util/meta.hpp>
#include <core/math/basic/constraint.hpp>
#include <core/math/exponential/sqrt.hpp>
#include <core/math/interpolation/map.hpp>
#include <core/math/trigonometric/trigonometric.hpp>

namespace legion::core::math
{
    // smoothstep
    template<typename TypeMin, typename TypeMax, typename ValueType>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto smoothstep(TypeMin&& edgeMin, TypeMax&& edgeMax, ValueType&& value) noexcept;

    // smoothstep
    template<typename ValueType>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto smoothstep(ValueType&& value) noexcept;

    // smoothstep_derivative
    template<typename TypeMin, typename TypeMax, typename ValueType>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto smoothstep_derivative(TypeMin&& edgeMin, TypeMax&& edgeMax, ValueType&& value) noexcept;

    // smoothstep_derivative
    template<typename ValueType>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto smoothstep_derivative(ValueType&& value) noexcept;

    // inverse_smoothstep
    template<typename TypeMin, typename TypeMax, typename ValueType>
    L_NODISCARD L_ALWAYS_INLINE auto inverse_smoothstep(TypeMin&& edgeMin, TypeMax&& edgeMax, ValueType&& value) noexcept;

    // inverse_smoothstep
    template<typename ValueType>
    L_NODISCARD L_ALWAYS_INLINE auto inverse_smoothstep(ValueType&& value) noexcept;

    // inverse_smoothstep_derivative
    template<typename TypeMin, typename TypeMax, typename ValueType>
    L_NODISCARD L_ALWAYS_INLINE auto inverse_smoothstep_derivative(TypeMin&& edgeMin, TypeMax&& edgeMax, ValueType&& value) noexcept;

    // inverse_smoothstep_derivative
    template<typename ValueType>
    L_NODISCARD L_ALWAYS_INLINE auto inverse_smoothstep_derivative(ValueType&& value) noexcept;
}

#include <core/math/interpolation/smoothstep.inl>
