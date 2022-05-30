#pragma once
#include <core/math/arithmetic/multiplication.hpp>

namespace legion::core::math::detail
{
    template<typename T>
    struct compute_multiplication;

    template<typename Scalar, size_type RowCount, size_type ColCount>
    struct compute_multiplication<matrix<Scalar, RowCount, ColCount>>
    {
        using mat_type = matrix<Scalar, RowCount, ColCount>;
        using vec_type = typename mat_type::row_type;

        template<typename other_type, std::enable_if_t<is_matrix_v<other_type>, bool> = true>
        L_NODISCARD constexpr static auto compute(const mat_type& a, const other_type& b) noexcept
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

        L_NODISCARD constexpr static auto compute(const vec_type& a, const mat_type& b) noexcept
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
    };
}
