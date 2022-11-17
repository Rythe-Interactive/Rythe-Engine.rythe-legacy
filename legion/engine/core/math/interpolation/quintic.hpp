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
    // quintic
    template<typename TypeMin, typename TypeMax, typename ValueType>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto quintic(TypeMin&& edgeMin, TypeMax&& edgeMax, ValueType&& value) noexcept;

    // quintic
    template<typename ValueType>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto quintic(ValueType&& value) noexcept;

    // quintic_derivative
    template<typename TypeMin, typename TypeMax, typename ValueType>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto quintic_derivative(TypeMin&& edgeMin, TypeMax&& edgeMax, ValueType&& value) noexcept;

    // quintic_derivative
    template<typename ValueType>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto quintic_derivative(ValueType&& value) noexcept;

    // inverse_quintic
    template<typename TypeMin, typename TypeMax, typename ValueType>
    L_NODISCARD L_ALWAYS_INLINE auto inverse_quintic(TypeMin&& edgeMin, TypeMax&& edgeMax, ValueType&& value) noexcept;

    // inverse_quintic
    template<typename ValueType>
    L_NODISCARD L_ALWAYS_INLINE auto inverse_quintic(ValueType&& value) noexcept;

    // inverse_quintic_derivative
    template<typename TypeMin, typename TypeMax, typename ValueType>
    L_NODISCARD L_ALWAYS_INLINE auto inverse_quintic_derivative(TypeMin&& edgeMin, TypeMax&& edgeMax, ValueType&& value) noexcept;

    // inverse_quintic_derivative
    template<typename ValueType>
    L_NODISCARD L_ALWAYS_INLINE auto inverse_quintic_derivative(ValueType&& value) noexcept;
}

#include <core/math/interpolation/quintic.inl>
