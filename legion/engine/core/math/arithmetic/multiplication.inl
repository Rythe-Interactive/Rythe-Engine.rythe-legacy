#include <core/math/arithmetic/multiplication.hpp>
#pragma once
#include <immintrin.h>

#include <core/math/arithmetic/multiplication_vector.inl>
#include <core/math/arithmetic/multiplication_matrix.inl>
#include <core/math/arithmetic/multiplication_quaternion.inl>

namespace legion::core::math
{
    // mul
    template<typename TypeA, typename TypeB>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto mul(TypeA&& a, TypeB&& b) noexcept
    {
        using A = remove_cvr_t<TypeA>;
        using B = remove_cvr_t<TypeB>;

        if constexpr (is_quat_v<A> && is_quat_v<B>)
        {
            using scalar = elevated_t<typename A::scalar, typename B::scalar>;
            return detail::compute_multiplication<quaternion<scalar>>::compute(std::forward<TypeA>(a), std::forward<TypeB>(b));
        }
        else if constexpr (is_vector_v<A> && is_quat_v<B>)
        {
            return detail::compute_multiplication<make_quat_t<B>>::compute(std::forward<TypeA>(a), std::forward<TypeB>(b));
        }
        else if constexpr (is_matrix_v<A> && is_matrix_v<B>)
        {
            using scalar = elevated_t<typename A::scalar, typename B::scalar>;
            return detail::compute_multiplication<matrix<scalar, A::row_count, A::col_count>>::compute(std::forward<TypeA>(a), std::forward<TypeB>(b));
        }
        else if constexpr (is_vector_v<A> && is_matrix_v<B>)
        {
            return detail::compute_multiplication<make_matrix_t<B>>::compute(std::forward<TypeA>(a), std::forward<TypeB>(b));
        }
        else if constexpr (is_vector_v<A> && is_vector_v<B>)
        {
            using scalar = elevated_t<typename A::scalar, typename B::scalar>;
            constexpr size_type size = min(A::size, B::size);
            return detail::compute_multiplication<vector<scalar, size>>::compute(std::forward<TypeA>(a), std::forward<TypeB>(b));
        }
        else
        {
            static_assert(is_vector_v<A> || is_vector_v<B>, "Invalid multiplication arguments");
            return detail::compute_multiplication<vector_type_t<A, B>>::compute(std::forward<TypeA>(a), std::forward<TypeB>(b));
        }
    }

    // operator *
    template<typename TypeA, typename TypeB, std::enable_if_t<is_linear_algebraic_construct_v<TypeA> || is_linear_algebraic_construct_v<TypeB>, bool>>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto operator*(TypeA&& a, TypeB&& b) noexcept
    {
        return mul(std::forward<TypeA>(a), std::forward<TypeB>(b));
    }

    // mul_assign
    template<typename TypeA, typename TypeB>
    L_ALWAYS_INLINE constexpr TypeA& mul_assign(TypeA& a, TypeB&& b) noexcept
    {
        return a = a * b;
    }

    // operator *=
    template<typename TypeA, typename TypeB, std::enable_if_t<is_linear_algebraic_construct_v<TypeA> || is_linear_algebraic_construct_v<TypeB>, bool>>
    L_ALWAYS_INLINE constexpr TypeA& operator*=(TypeA& a, TypeB&& b) noexcept
    {
        return a = a * b;
    }

}
