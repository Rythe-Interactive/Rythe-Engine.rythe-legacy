#include <core/math/trigonometric/cos.hpp>
#pragma once

#include <core/math/trigonometric/cos_vector.inl>
//#include <core/math/trigonometric/cos_matrix.inl>
//#include <core/math/trigonometric/cos_quaternion.inl>

namespace legion::core::math
{
    // cos
    template<typename T>
    L_NODISCARD L_ALWAYS_INLINE auto cos(T&& v) noexcept
    {
        using Type = remove_cvr_t<T>;

        if constexpr (is_quat_v<Type>)
        {
            return detail::compute_cos<quaternion<Type::scalar>>::compute(::std::forward<T>(value));
        }
        if constexpr (is_matrix_v<Type>)
        {
            return detail::compute_cos<matrix<Type::scalar, Type::row_count, Type::col_count>>::compute(::std::forward<T>(value));
        }
        if constexpr (is_vector_v<Type>)
        {
            return detail::compute_cos<vector<Type::scalar, Type::size>>::compute(::std::forward<T>(value));
        }
        else
        {
            return ::std::cos(::std::forward<T>(value));
        }
    }

    // acos
    template<typename T>
    L_NODISCARD L_ALWAYS_INLINE auto acos(T&& v) noexcept
    {
        using Type = remove_cvr_t<T>;

        if constexpr (is_quat_v<Type>)
        {
            return detail::compute_cos<quaternion<Type::scalar>>::compute_inverse(::std::forward<T>(value));
        }
        if constexpr (is_matrix_v<Type>)
        {
            return detail::compute_cos<matrix<Type::scalar, Type::row_count, Type::col_count>>::compute_inverse(::std::forward<T>(value));
        }
        if constexpr (is_vector_v<Type>)
        {
            return detail::compute_cos<vector<Type::scalar, Type::size>>::compute_inverse(::std::forward<T>(value));
        }
        else
        {
            return ::std::acos(::std::forward<T>(value));
        }
    }
}
