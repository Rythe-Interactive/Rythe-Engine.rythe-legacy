#pragma once
#include <core/math/matrix/matrix_base.hpp>
#include <core/math/matrix/column/column_base.hpp>
#include <core/math/vector/vector2.hpp>

namespace legion::core::math
{
    template<typename Scalar>
    struct quaternion;

    template<typename Scalar>
    struct matrix<Scalar, 2, 2> : matrix_base
    {
        using scalar = Scalar;
        static constexpr size_type row_count = 2;
        static constexpr size_type col_count = 2;
        static constexpr size_type size = row_count * col_count;
        using type = matrix<Scalar, 2, 2>;

        using row_type = vector<scalar, col_count>;

        template<size_type idx>
        using col_type = column<scalar, row_count, col_count, idx>;

        union
        {
            row_type rows[row_count];

            struct
            {
                row_type row0, row1;
            };

            scalar data[size];
            col_type<0> col0;
            col_type<1> col1;
        };

        static const matrix identity;
        static const matrix zero;

        constexpr matrix() noexcept :
            row0(float2::right),
            row1(float2::up)
        {}

        constexpr matrix(const matrix&) noexcept = default;

        explicit constexpr matrix(scalar s) noexcept :
            row0(s, static_cast<scalar>(0)),
            row1(static_cast<scalar>(0), s)
        {}

        explicit constexpr matrix(
            scalar s00, scalar s01,
            scalar s10, scalar s11) noexcept :
            row0(s00, s01),
            row1(s10, s11)
        {}

        explicit constexpr matrix(row_type r0, row_type r1) noexcept : row0(r0), row1(r1) {}

        template<typename Scal>
        explicit constexpr matrix(const quaternion<Scal>& orientation) noexcept;

        template<typename Scal, ::std::enable_if_t<!::std::is_same_v<scalar, Scal>, bool> = true>
        constexpr explicit matrix(const matrix<Scal, row_count, col_count>& other) noexcept;

        template<typename mat_type, ::std::enable_if_t<2 != mat_type::row_count || 2 != mat_type::col_count, bool> = true>
        constexpr matrix(const mat_type& other) noexcept;

        constexpr matrix& operator=(const matrix&) noexcept = default;

        L_NODISCARD constexpr row_type& operator[](size_type i) noexcept;
        L_NODISCARD constexpr const row_type& operator[](size_type i) const noexcept;
    };

    using float2x2 = matrix<float32, 2, 2>;
    using double2x2 = matrix<float64, 2, 2>;
}

#include <core/math/quaternion/matrix_quat_conv.inl>
