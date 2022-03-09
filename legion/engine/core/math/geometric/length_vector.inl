#pragma once
#include <core/math/geometric/length.hpp>

namespace legion::core::math::detail
{
    template<typename T>
    struct compute_length;

    template<typename Scalar, size_type Size>
    struct compute_length<vector<Scalar, Size>>
    {
        using vec_type = vector<Scalar, Size>;

        L_NODISCARD L_ALWAYS_INLINE static Scalar compute(const vec_type& v) noexcept
        {
            return ::std::sqrt(dot(v, v));
        }

        L_NODISCARD L_ALWAYS_INLINE constexpr static Scalar compute2(const vec_type& v) noexcept
        {
            return dot(v, v);
        }
    };

    template<typename Scalar>
    struct compute_length<vector<Scalar, 1u>>
    {
        using vec_type = vector<Scalar, 1u>;

        L_NODISCARD L_ALWAYS_INLINE static Scalar compute(Scalar v) noexcept
        {
            return v;
        }

        L_NODISCARD L_ALWAYS_INLINE constexpr static Scalar compute2(Scalar v) noexcept
        {
            return v * v;
        }
    };

    template<typename Scalar, size_type Size, size_type... args>
    struct compute_length<swizzle<Scalar, Size, args...>>
    {
        using swizzle_type = swizzle<Scalar, Size, args...>;
        using vec_type = typename swizzle_type::conv_type;

        L_NODISCARD L_ALWAYS_INLINE static auto compute(const vec_type& v) noexcept
        {
            return compute_length<vec_type>::compute(v);
        }

        L_NODISCARD L_ALWAYS_INLINE static auto compute2(const vec_type& v) noexcept
        {
            return compute_length<vec_type>::compute(v);
        }
    };
}
