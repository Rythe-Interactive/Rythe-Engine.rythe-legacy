#pragma once
#include <core/types/primitives.hpp>
#include <core/math/vector/vector_base.hpp>

namespace legion::core::math
{
    template<typename Scalar, size_type RowCount, size_type ColCount>
    struct matrix
    {
        static_assert(std::is_arithmetic_v<Scalar>, "Scalar must be a numeric type.");

        using scalar = Scalar;
        static constexpr size_type size = RowCount * ColCount;
        static constexpr size_type row_count = RowCount;
        static constexpr size_type col_count = ColCount;
        using type = matrix<Scalar, RowCount, ColCount>;

        using row_type = vector<scalar, col_count>;

        union
        {
            row_type rows[row_count];
            scalar data[row_count * col_count];
        };

        static const matrix identity;
        static const matrix zero;

        constexpr matrix() noexcept;

        constexpr matrix(const matrix&) noexcept = default;
        explicit constexpr matrix(scalar s) noexcept;

        template<typename Scal, ::std::enable_if_t<!::std::is_same_v<scalar, Scal>, bool> = true>
        constexpr explicit matrix(const matrix<Scal, row_count, col_count>&other) noexcept;

        template<typename mat_type, ::std::enable_if_t<RowCount != mat_type::row_count || ColCount != mat_type::col_count, bool> = true>
        constexpr matrix(const mat_type& other) noexcept;

        constexpr matrix& operator=(const matrix&) noexcept = default;

        L_NODISCARD constexpr row_type& operator[](size_type i) noexcept(!LEGION_VALIDATION_LEVEL);
        L_NODISCARD constexpr const row_type& operator[](size_type i) const noexcept(!LEGION_VALIDATION_LEVEL);
    };
}
