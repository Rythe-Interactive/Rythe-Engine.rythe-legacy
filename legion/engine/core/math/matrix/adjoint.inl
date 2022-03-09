#include <core/math/matrix/adjoint.hpp>
#pragma once

namespace legion::core::math
{
    namespace detail
    {
        template<typename T>
        struct compute_adjoint;

        template<typename Scalar, size_type RowCount, size_type ColCount>
        struct compute_adjoint<matrix<Scalar, RowCount, ColCount>>
        {
            using mat_type = matrix<Scalar, RowCount, ColCount>;
            using sub_mat_type = matrix<Scalar, RowCount - 1, ColCount - 1>;

            L_NODISCARD L_ALWAYS_INLINE constexpr static auto compute(const mat_type& v) noexcept
            {
                static_assert(RowCount == ColCount, "Adjoint can only be calculated of square matrices");

                using fp_type = ::std::conditional_t<::std::is_floating_point_v<Scalar>, Scalar, float_max>;

                mat_type result;
                for (size_type i = 0; i < RowCount; i++)
                    for (size_type j = 0; j < ColCount; j++) {
                        auto subMat = extract_sub_mat(v, i, j);
                        fp_type s = ::std::pow(static_cast<fp_type>(-1), static_cast<fp_type>(i + j));
                        result[i][j] = static_cast<Scalar>(s * determinant(subMat));
                    }

                return result;
            }
        };
    }

    template<typename mat_type, ::std::enable_if_t<is_matrix_v<mat_type>, bool>>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto adjoint(const mat_type& mat) noexcept
    {
        return detail::compute_adjoint<mat_type>::compute(mat);
    }
}
