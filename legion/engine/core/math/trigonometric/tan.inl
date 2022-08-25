#include <core/math/trigonometric/tan.hpp>
#pragma once

#include <core/math/trigonometric/tan_vector.inl>
//#include <core/math/trigonometric/tan_matrix.inl>
//#include <core/math/trigonometric/tan_quaternion.inl>

namespace legion::core::math
{
    // tan
    template<typename T>
    L_NODISCARD L_ALWAYS_INLINE auto tan(T&& v) noexcept
    {
        using Type = remove_cvr_t<T>;

        if constexpr (is_quat_v<Type>)
        {
            return detail::compute_tan<quaternion<Type::scalar>>::compute(::std::forward<T>(value));
        }
        if constexpr (is_matrix_v<Type>)
        {
            return detail::compute_tan<matrix<Type::scalar, Type::row_count, Type::col_count>>::compute(::std::forward<T>(value));
        }
        if constexpr (is_vector_v<Type>)
        {
            return detail::compute_tan<vector<Type::scalar, Type::size>>::compute(::std::forward<T>(value));
        }
        else
        {
            return ::std::tan(::std::forward<T>(value));
        }
    }

    // atan
    template<typename T>
    L_NODISCARD L_ALWAYS_INLINE auto atan(T&& v) noexcept
    {
        using Type = remove_cvr_t<T>;

        if constexpr (is_quat_v<Type>)
        {
            return detail::compute_tan<quaternion<Type::scalar>>::compute_inverse(::std::forward<T>(value));
        }
        if constexpr (is_matrix_v<Type>)
        {
            return detail::compute_tan<matrix<Type::scalar, Type::row_count, Type::col_count>>::compute_inverse(::std::forward<T>(value));
        }
        if constexpr (is_vector_v<Type>)
        {
            return detail::compute_tan<vector<Type::scalar, Type::size>>::compute_inverse(::std::forward<T>(value));
        }
        else
        {
            return ::std::atan(::std::forward<T>(value));
        }
    }
}
