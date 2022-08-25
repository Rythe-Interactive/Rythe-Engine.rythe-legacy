#include <core/math/trigonometric/sin.hpp>
#pragma once

#include <core/math/trigonometric/sin_vector.inl>
//#include <core/math/trigonometric/sin_matrix.inl>
//#include <core/math/trigonometric/sin_quaternion.inl>

namespace legion::core::math
{
    // sin
    template<typename T>
    L_NODISCARD L_ALWAYS_INLINE auto sin(T&& v) noexcept
    {
        using Type = remove_cvr_t<T>;

        if constexpr (is_quat_v<Type>)
        {
            return detail::compute_sin<quaternion<Type::scalar>>::compute(::std::forward<T>(value));
        }
        if constexpr (is_matrix_v<Type>)
        {
            return detail::compute_sin<matrix<Type::scalar, Type::row_count, Type::col_count>>::compute(::std::forward<T>(value));
        }
        if constexpr (is_vector_v<Type>)
        {
            return detail::compute_sin<vector<Type::scalar, Type::size>>::compute(::std::forward<T>(value));
        }
        else
        {
            return ::std::sin(::std::forward<T>(value));
        }
    }

    // asin
    template<typename T>
    L_NODISCARD L_ALWAYS_INLINE auto asin(T&& v) noexcept
    {
        using Type = remove_cvr_t<T>;

        if constexpr (is_quat_v<Type>)
        {
            return detail::compute_sin<quaternion<Type::scalar>>::compute_inverse(::std::forward<T>(value));
        }
        if constexpr (is_matrix_v<Type>)
        {
            return detail::compute_sin<matrix<Type::scalar, Type::row_count, Type::col_count>>::compute_inverse(::std::forward<T>(value));
        }
        if constexpr (is_vector_v<Type>)
        {
            return detail::compute_sin<vector<Type::scalar, Type::size>>::compute_inverse(::std::forward<T>(value));
        }
        else
        {
            return ::std::asin(::std::forward<T>(value));
        }
    }
}
