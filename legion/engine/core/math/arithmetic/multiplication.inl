#include <core/math/arithmetic/multiplication.hpp>
#pragma once
#include <immintrin.h>

namespace legion::core::math
{
    namespace detail
    {
        template<typename Scalar, size_type Size>
        L_NODISCARD constexpr auto compute_multiplication<vector<Scalar, Size>>::compute(const vec_type& a, const vec_type& b) noexcept
        {
            vec_type result;
            for (size_type i = 0; i < Size; i++)
                result[i] = a[i] * b[i];
            return result;
        }

        template<typename Scalar, size_type Size>
        L_NODISCARD constexpr auto compute_multiplication<vector<Scalar, Size>>::compute(const vec_type& a, Scalar b) noexcept
        {
            vec_type result;
            for (size_type i = 0; i < Size; i++)
                result[i] = a[i] * b;
            return result;
        }


        template<typename Scalar>
        L_NODISCARD constexpr Scalar compute_multiplication<vector<Scalar, 1u>>::compute(const vec_type& a, Scalar b) noexcept
        {
            return a[0] * b;
        }

        template<typename Scalar, size_type RowCount, size_type ColCount>
        template<typename other_type, std::enable_if_t<is_matrix_v<other_type>, bool>>
        L_NODISCARD constexpr auto compute_multiplication<matrix<Scalar, RowCount, ColCount>>::compute(const mat_type& a, const other_type& b) noexcept
        {
            static_assert(ColCount == other_type::row_count, "Matrix/matrix multiplication requires the column count of the first to be the same as the row count of the second.");

            using scalar = lowest_precision_t<Scalar, typename other_type::scalar>;
            using result_type = matrix<scalar, RowCount, other_type::col_count>;

            result_type result = result_type::zero;

            for (size_type i = 0; i < RowCount; i++)
                for (size_type j = 0; j < other_type::col_count; j++)
                    for (size_type k = 0; k < other_type::row_count; k++)
                        result[i][j] += a[i][k] * b[k][j];

            return result;
        }

        template<typename Scalar, size_type RowCount, size_type ColCount>
        L_NODISCARD constexpr auto compute_multiplication<matrix<Scalar, RowCount, ColCount>>::compute(const vec_type& a, const mat_type& b) noexcept
        {
            static_assert(ColCount == vec_type::size, "Matrix/vector multiplication requires vector to have the same size as the number of columns");
            using scalar = lowest_precision_t<remove_cvr_t<Scalar>, remove_cvr_t<typename vec_type::scalar>>;
            using result_type = vector<scalar, RowCount>;

            result_type result = result_type::zero;

            for (size_type i = 0; i < RowCount; i++)
                for (size_type j = 0; j < ColCount; j++)
                    result[i] += (b[i][j] * a[j]);

            return result;
        }
    }

    template<typename vec_type0, typename vec_type1, std::enable_if_t<is_vector_v<vec_type0>&& is_vector_v<vec_type1>, bool>>
    L_NODISCARD constexpr auto mul(const vec_type0& a, const vec_type1& b) noexcept
    {
        return detail::compute_multiplication<vec_type0>::compute(a, b);
    }

    template<typename vec_type0, typename vec_type1, std::enable_if_t<is_vector_v<vec_type0>&& is_vector_v<vec_type1>, bool>>
    L_NODISCARD constexpr auto operator*(const vec_type0& a, const vec_type1& b) noexcept
    {
        return detail::compute_multiplication<vec_type0>::compute(a, b);
    }

    template<typename vec_type, std::enable_if_t<is_vector_v<vec_type>, bool>>
    L_NODISCARD constexpr auto mul(const vec_type& a, typename vec_type::scalar b) noexcept
    {
        return detail::compute_multiplication<vec_type>::compute(a, b);
    }

    template<typename vec_type, std::enable_if_t<is_vector_v<vec_type>, bool>>
    L_NODISCARD constexpr auto operator*(const vec_type& a, typename vec_type::scalar b) noexcept
    {
        return detail::compute_multiplication<vec_type>::compute(a, b);
    }

    template<typename vec_type0, typename vec_type1, std::enable_if_t<is_vector_v<vec_type0>&& is_vector_v<vec_type1>, bool>>
    constexpr vec_type0& mul_assign(vec_type0& a, const vec_type1& b) noexcept
    {
        return a = detail::compute_multiplication<vec_type0>::compute(a, b);
    }

    template<typename vec_type0, typename vec_type1, std::enable_if_t<is_vector_v<vec_type0>&& is_vector_v<vec_type1>, bool>>
    constexpr vec_type0& operator*=(vec_type0& a, const vec_type1& b) noexcept
    {
        return a = detail::compute_multiplication<vec_type0>::compute(a, b);
    }

    template<typename vec_type, std::enable_if_t<is_vector_v<vec_type>, bool>>
    constexpr vec_type& mul_assign(vec_type& a, typename vec_type::scalar b) noexcept
    {
        return a = detail::compute_multiplication<vec_type>::compute(a, b);
    }

    template<typename vec_type, std::enable_if_t<is_vector_v<vec_type>, bool>>
    constexpr vec_type& operator*=(vec_type& a, typename vec_type::scalar b) noexcept
    {
        return a = detail::compute_multiplication<vec_type>::compute(a, b);
    }

    template<typename mat_type0, typename mat_type1, std::enable_if_t<is_matrix_v<mat_type0>&& is_matrix_v<mat_type1>, bool>>
    L_NODISCARD constexpr auto mul(const mat_type0& a, const mat_type1& b) noexcept
    {
        return detail::compute_multiplication<mat_type0>::compute(a, b);
    }

    template<typename mat_type0, typename mat_type1, std::enable_if_t<is_matrix_v<mat_type0>&& is_matrix_v<mat_type1>, bool>>
    L_NODISCARD constexpr auto operator*(const mat_type0& a, const mat_type1& b) noexcept
    {
        return detail::compute_multiplication<mat_type0>::compute(a, b);
    }

    template<typename mat_type, typename vec_type, std::enable_if_t<is_matrix_v<mat_type>&& is_vector_v<vec_type>, bool>>
    L_NODISCARD constexpr auto mul(const vec_type& a, const mat_type& b) noexcept
    {
        return detail::compute_multiplication<mat_type>::compute(a, b);
    }

    template<typename mat_type, typename vec_type, std::enable_if_t<is_matrix_v<mat_type>&& is_vector_v<vec_type>, bool>>
    L_NODISCARD constexpr auto operator*(const vec_type& a, const mat_type& b) noexcept
    {
        return detail::compute_multiplication<mat_type>::compute(a, b);
    }

    template<typename mat_type0, typename mat_type1, std::enable_if_t<is_matrix_v<mat_type0>&& is_matrix_v<mat_type1>, bool>>
    constexpr mat_type0& mul_assign(mat_type0& a, const mat_type1& b) noexcept
    {
        static_assert(mat_type0::row_count == mat_type1::row_count && mat_type0::col_count == mat_type1::col_count, "For multiply assign, both matrices need to have identical dimensions.");
        return a = detail::compute_multiplication<mat_type0>::compute(a, b);
    }

    template<typename mat_type0, typename mat_type1, std::enable_if_t<is_matrix_v<mat_type0>&& is_matrix_v<mat_type1>, bool>>
    constexpr mat_type0& operator*=(mat_type0& a, const mat_type1& b) noexcept
    {
        static_assert(mat_type0::row_count == mat_type1::row_count && mat_type0::col_count == mat_type1::col_count, "For multiply assign, both matrices need to have identical dimensions.");
        return a = detail::compute_multiplication<mat_type0>::compute(a, b);
    }

}
