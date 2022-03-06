#include <core/math/matrix/adjoint.hpp>
#pragma once

namespace legion::core::math
{
    //void adjoint(int a[N][N], int adj[N][N]) {
//    int s = 1;
//    int co_fact[N][N];
//    for (int i = 0; i < N; i++) {
//        for (int j = 0; j < N; j++) {
//            getCoFactor(a, co_fact, i, j, N);
//            s = pow(-1, (i + j));
//            adj[j][i] = s * determinant(co_fact, N - 1);
//        }
//    }
//}

    namespace detail
    {
        template<typename T>
        struct compute_adjoint;

        template<typename Scalar, size_type RowCount, size_type ColCount>
        struct compute_adjoint<matrix<Scalar, RowCount, ColCount>>
        {
            using mat_type = matrix<Scalar, RowCount, ColCount>;
            using sub_mat_type = matrix<Scalar, RowCount - 1, ColCount - 1>;

            L_NODISCARD L_ALWAYS_INLINE constexpr static Scalar compute(const mat_type& v) noexcept
            {
                static_assert(RowCount == ColCount, "Adjoint can only be calculated of square matrices");

                mat_type result;
                return result;
            }
        };
    }

    template<typename mat_type, ::std::enable_if_t<is_matrix_v<mat_type>, bool>>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto determinant(const mat_type& mat) noexcept
    {
        return detail::compute_determinant<mat_type>::compute(mat);
    }
}
