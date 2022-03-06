#include <core/math/matrix/determinant.hpp>
#pragma once

namespace legion::core::math
{
    namespace detail
    {
        template<typename Scalar, size_type RowCount, size_type ColCount>
        L_NODISCARD constexpr auto compute_determinant<matrix<Scalar, RowCount, ColCount>>::extract_sub_mat(const mat_type& v, size_type idx)
        {
            sub_mat_type result = sub_mat_type::zero;
            size_type resultRow = 0;
            size_type resultCol = 0;

            for (size_type srcRow = 1; srcRow < RowCount; srcRow++)
                for (size_type srcCol = 0; srcCol < ColCount; srcCol++)
                {
                    if (srcCol == idx)
                        continue;

                    if (resultCol >= ColCount)
                    {
                        resultCol = 0;
                        resultRow++;
                    }

                    result[resultRow][resultCol++] = v[srcRow][srcCol];
                }

            return result;
        }

        template<typename Scalar, size_type RowCount, size_type ColCount>
        L_NODISCARD constexpr Scalar compute_determinant<matrix<Scalar, RowCount, ColCount>>::compute(const mat_type& v) noexcept
        {
            static_assert(RowCount == ColCount, "Determinants can only be calculated of square matrices");

            Scalar result = static_cast<Scalar>(0);
            for (size_type i = 0; i < ColCount; i++)
                result += static_cast<Scalar>(i % 2 ? -1 : 1) * v[0][i] * compute_determinant<sub_mat_type>::compute(extract_sub_mat(v, i));
            return result;
        }

        template<typename Scalar>
        L_NODISCARD constexpr Scalar compute_determinant<matrix<Scalar, 1, 1>>::compute(const mat_type& v) noexcept
        {
            return v[0][0];
        }

        template<typename Scalar>
        L_NODISCARD constexpr Scalar compute_determinant<matrix<Scalar, 2, 2>>::compute(const mat_type& v) noexcept
        {
            return v[0][0] * v[1][1] - v[0][1] * v[1][0];
        }
    }

    template<typename mat_type, ::std::enable_if_t<is_matrix_v<mat_type>, bool>>
    L_NODISCARD constexpr auto determinant(const mat_type& mat) noexcept
    {
        return detail::compute_determinant<mat_type>::compute(mat);
    }
}
