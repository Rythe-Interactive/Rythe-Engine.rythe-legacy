#pragma once
#include <core/math/geometric/normalize.hpp>

namespace legion::core::math::detail
{
    template<typename T>
    struct compute_normalize;

    template<typename Scalar, size_type Size>
    struct compute_normalize<vector<Scalar, Size>>
    {
        using vec_type = vector<Scalar, Size>;

        L_NODISCARD L_ALWAYS_INLINE static vec_type compute(const vec_type& v) noexcept
        {
            return v / length(v);
        }
    };

    template<typename Scalar>
    struct compute_normalize<vector<Scalar, 1u>>
    {
        using vec_type = vector<Scalar, 1u>;

        L_NODISCARD L_ALWAYS_INLINE static Scalar compute(L_MAYBEUNUSED Scalar v) noexcept
        {
            return static_cast<Scalar>(1);
        }
    };
}
