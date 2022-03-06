#pragma once
#include <core/math/arithmetic/multiplication.hpp>

namespace legion::core::math::detail
{
    template<typename T>
    struct compute_multiplication;

    template<typename Scalar, size_type Size>
    struct compute_multiplication<vector<Scalar, Size>>
    {
        using vec_type = vector<Scalar, Size>;

        L_NODISCARD constexpr static auto compute(const vec_type& a, const vec_type& b) noexcept
        {
            vec_type result;
            for (size_type i = 0; i < Size; i++)
                result[i] = a[i] * b[i];
            return result;
        }

        L_NODISCARD constexpr static auto compute(const vec_type& a, Scalar b) noexcept
        {
            vec_type result;
            for (size_type i = 0; i < Size; i++)
                result[i] = a[i] * b;
            return result;
        }
    };

    template<typename Scalar>
    struct compute_multiplication<vector<Scalar, 1u>>
    {
        using vec_type = vector<Scalar, 1u>;

        L_NODISCARD constexpr static Scalar compute(const vec_type& a, Scalar b) noexcept
        {
            return a[0] * b;
        }
    };

    template<typename Scalar, size_type Size, size_type... args>
    struct compute_multiplication<swizzle<Scalar, Size, args...>>
    {
        using swizzle_type = swizzle<Scalar, Size, args...>;
        using vec_type = typename swizzle_type::conv_type;

        L_NODISCARD constexpr static auto compute(const vec_type& a, const vec_type& b) noexcept
        {
            return compute_multiplication<vec_type>::compute(a, b);
        }

        L_NODISCARD constexpr static auto compute(const vec_type& a, Scalar b) noexcept
        {
            return compute_multiplication<vec_type>::compute(a, b);
        }
    };

    template<typename Scalar, size_type Size, size_type arg>
    struct compute_multiplication<swizzle<Scalar, Size, arg>>
    {
        using swizzle_type = swizzle<Scalar, Size, arg>;
        using vec_type = typename swizzle_type::conv_type;

        L_NODISCARD constexpr static auto compute(const vec_type& a, Scalar b) noexcept
        {
            return compute_multiplication<vec_type>::compute(a, b);
        }
    };
}
