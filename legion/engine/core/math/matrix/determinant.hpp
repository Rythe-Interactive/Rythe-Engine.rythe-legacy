#pragma once
#include <core/types/primitives.hpp>
#include <core/math/matrix/matrix_base.hpp>
#include <core/math/meta.hpp>

namespace legion::core::math
{
    namespace detail
    {
        template<typename T>
        struct compute_determinant;

        template<typename Scalar, size_type RowCount, size_type ColCount>
        struct compute_determinant<matrix<Scalar, RowCount, ColCount>>
        {
            using mat_type = matrix<Scalar, RowCount, ColCount>;
            using sub_mat_type = matrix<Scalar, RowCount - 1, ColCount - 1>;

            L_NODISCARD constexpr static auto extract_sub_mat(const mat_type& v, size_type idx);
            L_NODISCARD constexpr static Scalar compute(const mat_type& v) noexcept;
        };

        template<typename Scalar>
        struct compute_determinant<matrix<Scalar, 1, 1>>
        {
            using mat_type = matrix<Scalar, 1, 1>;

            L_NODISCARD constexpr static Scalar compute(const mat_type& v) noexcept;
        };

        template<typename Scalar>
        struct compute_determinant<matrix<Scalar, 2, 2>>
        {
            using mat_type = matrix<Scalar, 2, 2>;

            L_NODISCARD constexpr static Scalar compute(const mat_type& v) noexcept;
        };
    }

    template<typename mat_type, ::std::enable_if_t<is_matrix_v<mat_type>, bool> = true>
    L_NODISCARD constexpr auto determinant(const mat_type& mat) noexcept;
}

#include <core/math/matrix/determinant.inl>
