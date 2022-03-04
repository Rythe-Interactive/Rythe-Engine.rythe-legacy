#include <core/math/matrix/matrix_base.hpp>
#pragma once

namespace legion::core::math
{
    template<typename Scalar, size_type RowCount, size_type ColCount>
    const matrix<Scalar, RowCount, ColCount> matrix<Scalar, RowCount, ColCount>::identity = matrix<Scalar, RowCount, ColCount>(static_cast<Scalar>(1));

    template<typename Scalar, size_type RowCount, size_type ColCount>
    const matrix<Scalar, RowCount, ColCount> matrix<Scalar, RowCount, ColCount>::zero = matrix<Scalar, RowCount, ColCount>(static_cast<Scalar>(0));

    template<typename Scalar, size_type RowCount, size_type ColCount>
    constexpr matrix<Scalar, RowCount, ColCount>::matrix() noexcept
    {
        for (size_type i = 0; i < row_count; i++)
            for (size_type j = 0; j < col_count; j++)
                rows[i][j] = i == j ? static_cast<scalar>(1) : static_cast<scalar>(0);
    }

    template<typename Scalar, size_type RowCount, size_type ColCount>
    constexpr matrix<Scalar, RowCount, ColCount>::matrix(scalar s) noexcept
    {
        for (size_type i = 0; i < row_count; i++)
            for (size_type j = 0; j < col_count; j++)
                rows[i][j] = i == j ? s : static_cast<scalar>(0);
    }

    template<typename Scalar, size_type RowCount, size_type ColCount>
    template<typename Scal, ::std::enable_if_t<!::std::is_same_v<Scalar, Scal>, bool>>
    constexpr matrix<Scalar, RowCount, ColCount>::matrix(const matrix<Scal, row_count, col_count>& other) noexcept
    {
        for (size_type i = 0; i < size; i++)
            data[i] = static_cast<scalar>(other.data[i]);
    }

    template<typename Scalar, size_type RowCount, size_type ColCount>
    template<typename mat_type, ::std::enable_if_t<RowCount != mat_type::row_count || ColCount != mat_type::col_count, bool>>
    constexpr matrix<Scalar, RowCount, ColCount>::matrix(const mat_type& other) noexcept
    {
        constexpr size_type min_row = row_count < mat_type::row_count ? row_count : mat_type::row_count;
        constexpr size_type min_col = col_count < mat_type::col_count ? col_count : mat_type::col_count;

        for (size_type i = 0; i < min_row; i++)
            for (size_type j = 0; j < min_col; j++)
                rows[i][j] = static_cast<scalar>(other.mx[i][j]);
    }

    template<typename Scalar, size_type RowCount, size_type ColCount>
    L_NODISCARD constexpr typename matrix<Scalar, RowCount, ColCount>::row_type& matrix<Scalar, RowCount, ColCount>::operator[](size_type i) noexcept(!LEGION_VALIDATION_LEVEL)
    {
        assert_msg("matrix subscript out of range", (i >= 0) && (i < row_count)); return rows[i];
    }

    template<typename Scalar, size_type RowCount, size_type ColCount>
    L_NODISCARD constexpr const typename matrix<Scalar, RowCount, ColCount>::row_type& matrix<Scalar, RowCount, ColCount>::operator[](size_type i) const noexcept(!LEGION_VALIDATION_LEVEL)
    {
        assert_msg("matrix subscript out of range", (i >= 0) && (i < row_count)); return rows[i];
    }
}
