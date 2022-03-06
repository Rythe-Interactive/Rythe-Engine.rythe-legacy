#include <core/math/arithmetic/multiplication.hpp>
#pragma once
#include <immintrin.h>

#include <core/math/arithmetic/multiplication_vector.inl>
#include <core/math/arithmetic/multiplication_matrix.inl>
#include <core/math/arithmetic/multiplication_quaternion.inl>

namespace legion::core::math
{
    template<typename vec_type0, typename vec_type1, std::enable_if_t<is_vector_v<vec_type0>&& is_vector_v<vec_type1>, bool>>
    L_NODISCARD constexpr auto mul(const vec_type0& a, const vec_type1& b) noexcept
    {
        return detail::compute_multiplication<vec_type0>::compute(a, b);
    }

    template<typename vec_type, std::enable_if_t<is_vector_v<vec_type>, bool>>
    L_NODISCARD constexpr auto mul(const vec_type& a, typename vec_type::scalar b) noexcept
    {
        return detail::compute_multiplication<vec_type>::compute(a, b);
    }

    template<typename mat_type0, typename mat_type1, std::enable_if_t<is_matrix_v<mat_type0>&& is_matrix_v<mat_type1>, bool>>
    L_NODISCARD constexpr auto mul(const mat_type0& a, const mat_type1& b) noexcept
    {
        return detail::compute_multiplication<mat_type0>::compute(a, b);
    }

    template<typename mat_type, typename vec_type, std::enable_if_t<is_matrix_v<mat_type>&& is_vector_v<vec_type>, bool>>
    L_NODISCARD constexpr auto mul(const vec_type& a, const mat_type& b) noexcept
    {
        return detail::compute_multiplication<mat_type>::compute(a, b);
    }

    template<typename quat_type0, typename quat_type1, std::enable_if_t<is_quat_v<quat_type0>&& is_quat_v<quat_type1>, bool>>
    L_NODISCARD constexpr auto mul(const quat_type0& a, const quat_type1& b) noexcept
    {
        using scalar = lowest_precision_t<typename quat_type0::scalar, typename quat_type1::scalar>;
        return detail::compute_multiplication<quaternion<scalar>>::compute(a, b);
    }

    template<typename quat_type, typename vec_type, std::enable_if_t<is_quat_v<quat_type>&& is_vector_v<vec_type>, bool>>
    L_NODISCARD constexpr auto mul(const vec_type& a, const quat_type& b) noexcept
    {
        return detail::compute_multiplication<quat_type>::compute(a, b);
    }

    template<typename vec_type0, typename vec_type1, std::enable_if_t<is_vector_v<vec_type0>&& is_vector_v<vec_type1>, bool>>
    L_NODISCARD constexpr auto operator*(const vec_type0& a, const vec_type1& b) noexcept
    {
        return detail::compute_multiplication<vec_type0>::compute(a, b);
    }

    template<typename vec_type, std::enable_if_t<is_vector_v<vec_type>, bool>>
    L_NODISCARD constexpr auto operator*(const vec_type& a, typename vec_type::scalar b) noexcept
    {
        return detail::compute_multiplication<vec_type>::compute(a, b);
    }

    template<typename mat_type0, typename mat_type1, std::enable_if_t<is_matrix_v<mat_type0>&& is_matrix_v<mat_type1>, bool>>
    L_NODISCARD constexpr auto operator*(const mat_type0& a, const mat_type1& b) noexcept
    {
        return detail::compute_multiplication<mat_type0>::compute(a, b);
    }

    template<typename mat_type, typename vec_type, std::enable_if_t<is_matrix_v<mat_type>&& is_vector_v<vec_type>, bool>>
    L_NODISCARD constexpr auto operator*(const vec_type& a, const mat_type& b) noexcept
    {
        return detail::compute_multiplication<mat_type>::compute(a, b);
    }

    template<typename quat_type0, typename quat_type1, std::enable_if_t<is_quat_v<quat_type0>&& is_quat_v<quat_type1>, bool>>
    L_NODISCARD constexpr auto operator*(const quat_type0& a, const quat_type1& b) noexcept
    {
        using scalar = lowest_precision_t<typename quat_type0::scalar, typename quat_type1::scalar>;
        return detail::compute_multiplication<quaternion<scalar>>::compute(a, b);
    }

    template<typename quat_type, typename vec_type, std::enable_if_t<is_quat_v<quat_type>&& is_vector_v<vec_type>, bool>>
    L_NODISCARD constexpr auto operator*(const vec_type& a, const quat_type& b) noexcept
    {
        return detail::compute_multiplication<vec_type>::compute(a, b);
    }

    template<typename vec_type0, typename vec_type1, std::enable_if_t<is_vector_v<vec_type0>&& is_vector_v<vec_type1>, bool>>
    constexpr vec_type0& mul_assign(vec_type0& a, const vec_type1& b) noexcept
    {
        return a = detail::compute_multiplication<vec_type0>::compute(a, b);
    }

    template<typename vec_type, std::enable_if_t<is_vector_v<vec_type>, bool>>
    constexpr vec_type& mul_assign(vec_type& a, typename vec_type::scalar b) noexcept
    {
        return a = detail::compute_multiplication<vec_type>::compute(a, b);
    }

    template<typename mat_type0, typename mat_type1, std::enable_if_t<is_matrix_v<mat_type0>&& is_matrix_v<mat_type1>, bool>>
    constexpr mat_type0& mul_assign(mat_type0& a, const mat_type1& b) noexcept
    {
        static_assert(mat_type0::row_count == mat_type1::row_count && mat_type0::col_count == mat_type1::col_count, "For multiply assign, both matrices need to have identical dimensions.");
        return a = detail::compute_multiplication<mat_type0>::compute(a, b);
    }

    template<typename mat_type, typename vec_type, std::enable_if_t<is_matrix_v<mat_type>&& is_vector_v<vec_type>, bool>>
    constexpr vec_type& mul_assign(vec_type& a, const mat_type& b) noexcept
    {
        return a = detail::compute_multiplication<mat_type>::compute(a, b);
    }

    template<typename quat_type0, typename quat_type1, std::enable_if_t<is_quat_v<quat_type0>&& is_quat_v<quat_type1>, bool>>
    constexpr quat_type0& mul_assign(quat_type0& a, const quat_type1& b) noexcept
    {
        return a = detail::compute_multiplication<quat_type0>::compute(a, b);
    }

    template<typename quat_type, typename vec_type, std::enable_if_t<is_quat_v<quat_type>&& is_vector_v<vec_type>, bool>>
    constexpr vec_type& mul_assign(vec_type& a, const quat_type& b) noexcept
    {
        return a = detail::compute_multiplication<quat_type>::compute(a, b);
    }

    template<typename vec_type0, typename vec_type1, std::enable_if_t<is_vector_v<vec_type0>&& is_vector_v<vec_type1>, bool>>
    constexpr vec_type0& operator*=(vec_type0& a, const vec_type1& b) noexcept
    {
        return a = detail::compute_multiplication<vec_type0>::compute(a, b);
    }

    template<typename vec_type, std::enable_if_t<is_vector_v<vec_type>, bool>>
    constexpr vec_type& operator*=(vec_type& a, typename vec_type::scalar b) noexcept
    {
        return a = detail::compute_multiplication<vec_type>::compute(a, b);
    }

    template<typename mat_type0, typename mat_type1, std::enable_if_t<is_matrix_v<mat_type0>&& is_matrix_v<mat_type1>, bool>>
    constexpr mat_type0& operator*=(mat_type0& a, const mat_type1& b) noexcept
    {
        static_assert(mat_type0::row_count == mat_type1::row_count && mat_type0::col_count == mat_type1::col_count, "For multiply assign, both matrices need to have identical dimensions.");
        return a = detail::compute_multiplication<mat_type0>::compute(a, b);
    }

    template<typename mat_type, typename vec_type, std::enable_if_t<is_matrix_v<mat_type>&& is_vector_v<vec_type>, bool>>
    constexpr vec_type& operator*=(vec_type& a, const mat_type& b) noexcept
    {
        return a = detail::compute_multiplication<mat_type>::compute(a, b);
    }

    template<typename quat_type0, typename quat_type1, std::enable_if_t<is_quat_v<quat_type0>&& is_quat_v<quat_type1>, bool>>
    constexpr quat_type0& operator*=(quat_type0& a, const quat_type1& b) noexcept
    {
        return a = detail::compute_multiplication<quat_type0>::compute(a, b);
    }

    template<typename quat_type, typename vec_type, std::enable_if_t<is_quat_v<quat_type>&& is_vector_v<vec_type>, bool>>
    constexpr vec_type& operator*=(vec_type& a, const quat_type& b) noexcept
    {
        return a = detail::compute_multiplication<quat_type>::compute(a, b);
    }

}
