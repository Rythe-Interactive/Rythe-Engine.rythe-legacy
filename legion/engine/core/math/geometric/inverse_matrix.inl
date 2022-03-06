#pragma once
#include <core/math/geometric/inverse.hpp>

namespace legion::core::math::detail
{
    //void Inverse(int A[N][N], float inverse[N][N]) {
    //    int deter = determinant(A);
    //    int adj[N][N];
    //    adjoint(A, adj); //Finding the adjoint of the matrix
    //    for (int i = 0; i < N; i++) {
    //        for (int j = 0; j < N; j++) {
    //            inverse[i][j] = adj[i][j] / (float)deter;
    //        }
    //    }
    //}


    template<typename T>
    struct compute_inverse;

    template<typename Scalar, size_type Size>
    struct compute_inverse<matrix<Scalar, Size, Size>>
    {
        using mat_type = matrix<Scalar, Size, Size>;

        L_NODISCARD L_ALWAYS_INLINE static mat_type compute(const mat_type& m) noexcept
        {
            
        }
    };
}
