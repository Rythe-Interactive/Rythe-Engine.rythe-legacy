#pragma once
#include <core/math/geometric/inverse.hpp>

namespace legion::core::math::detail
{
    template<typename T>
    struct compute_inverse;

    template<typename Scalar, size_type Size>
    struct compute_inverse<vector<Scalar, Size>>
    {
        using vec_type = vector<Scalar, Size>;

        L_NODISCARD L_ALWAYS_INLINE static vec_type compute(const vec_type& v) noexcept
        {
            return vec_type::one / v;
        }
    };

    template<typename Scalar>
    struct compute_inverse<vector<Scalar, 1u>>
    {
        using vec_type = vector<Scalar, 1u>;

        L_NODISCARD L_ALWAYS_INLINE static Scalar compute(Scalar v) noexcept
        {
            return static_cast<Scalar>(1) / v;
        }
    };

    template<typename Scalar, size_type Size, size_type... args>
    struct compute_inverse<swizzle<Scalar, Size, args...>>
    {
        using swizzle_type = swizzle<Scalar, Size, args...>;
        using vec_type = typename swizzle_type::conv_type;

        L_NODISCARD L_ALWAYS_INLINE static auto compute(const vec_type& v) noexcept
        {
            return compute_inverse<vec_type>::compute(v);
        }
    };
}
