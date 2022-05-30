#pragma once
#include <core/math/geometric/inverse.hpp>

namespace legion::core::math::detail
{
    template<typename T>
    struct compute_inverse;

    template<typename Scalar, size_type RowCount, size_type ColCount>
    struct compute_inverse<matrix<Scalar, RowCount, ColCount>>
    {
        using mat_type = matrix<Scalar, RowCount, ColCount>;

        L_NODISCARD L_ALWAYS_INLINE static mat_type compute(const mat_type& m) noexcept
        {
            static_assert(RowCount == ColCount, "Determinants can only be calculated of square matrices");

            mat_type result;
            auto det = determinant(m);
            auto adj = adjoint(m);
            for (size_type i = 0; i < RowCount; i++)
                for (size_type j = 0; j < ColCount; j++)
                    result[i][j] = adj[i][j] / det;

            return result;
        }
    };
}
