#pragma once
#include <core/math/matrix/matrix_base.hpp>
#include <core/math/matrix/column/column_base.hpp>
#include <core/math/vector/vector4.hpp>

namespace legion::core::math
{
    template<typename Scalar>
    struct matrix<Scalar, 4, 4>
    {
        static_assert(std::is_arithmetic_v<Scalar>, "Scalar must be a numeric type.");

        using scalar = Scalar;
        static constexpr size_type size = 4 * 4;
        static constexpr size_type row_count = 4;
        static constexpr size_type col_count = 4;
        using type = matrix<Scalar, 4, 4>;

        using row_type = vector<scalar, col_count>;

        template<size_type idx>
        using col_type = column<scalar, row_count, col_count, idx>;

        union
        {
            row_type rows[row_count];

            struct
            {
                row_type row0, row1, row2, row3;
            };

            scalar data[row_count * col_count];
            col_type<0> col0;
            col_type<1> col1;
            col_type<2> col2;
            col_type<3> col3;
        };

        static const matrix identity;

        constexpr matrix() noexcept :
            row0(float4::right),
            row1(float4::up),
            row2(float4::forward),
            row3(float4::positiveW)
        {}

        constexpr matrix(const matrix&) noexcept = default;

        explicit constexpr matrix(scalar s) noexcept :
            row0(s, static_cast<scalar>(0), static_cast<scalar>(0), static_cast<scalar>(0)),
            row1(static_cast<scalar>(0), s, static_cast<scalar>(0), static_cast<scalar>(0)),
            row2(static_cast<scalar>(0), static_cast<scalar>(0), s, static_cast<scalar>(0)),
            row3(static_cast<scalar>(0), static_cast<scalar>(0), static_cast<scalar>(0), s)
        {}

        template<typename Scal, ::std::enable_if_t<!::std::is_same_v<scalar, Scal>, bool> = true>
        constexpr explicit matrix(const matrix<Scal, row_count, col_count>& other) noexcept;

        template<typename mat_type, ::std::enable_if_t<4 != mat_type::row_count || 4 != mat_type::col_count, bool> = true>
        constexpr matrix(const mat_type& other) noexcept;

        constexpr matrix& operator=(const matrix&) noexcept = default;

        L_NODISCARD constexpr row_type& operator[](size_type i) noexcept(!LEGION_VALIDATION_LEVEL);
        L_NODISCARD constexpr const row_type& operator[](size_type i) const noexcept(!LEGION_VALIDATION_LEVEL);
    };

    using float4x4 = matrix<float32, 4, 4>;
    using double4x4 = matrix<float64, 4, 4>;
}
