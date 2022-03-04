#pragma once
#include <core/types/primitives.hpp>
#include <core/math/vector/vector.hpp>
#include <core/math/matrix/matrix.hpp>
#include <core/math/meta.hpp>

namespace legion::core::math
{
    namespace detail
    {
        template<typename T>
        struct compute_multiplication;

        template<typename Scalar, size_type Size>
        struct compute_multiplication<vector<Scalar, Size>>
        {
            using vec_type = vector<Scalar, Size>;

            L_NODISCARD constexpr static auto compute(const vec_type& a, const vec_type& b) noexcept;

            L_NODISCARD constexpr static auto compute(const vec_type& a, Scalar b) noexcept;
        };

        template<typename Scalar>
        struct compute_multiplication<vector<Scalar, 1u>>
        {
            using vec_type = vector<Scalar, 1u>;

            L_NODISCARD constexpr static Scalar compute(const vec_type& a, Scalar b) noexcept;
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

        template<typename Scalar, size_type RowCount, size_type ColCount>
        struct compute_multiplication<matrix<Scalar, RowCount, ColCount>>
        {
            using mat_type = matrix<Scalar, RowCount, ColCount>;
            using vec_type = typename mat_type::row_type;

            template<typename other_type, std::enable_if_t<is_matrix_v<other_type>, bool> = true>
            L_NODISCARD constexpr static auto compute(const mat_type& a, const other_type& b) noexcept;

            L_NODISCARD constexpr static auto compute(const vec_type& a, const mat_type& b) noexcept;
        };

    }

    // Vector
    template<typename vec_type0, typename vec_type1, std::enable_if_t<is_vector_v<vec_type0> && is_vector_v<vec_type1>, bool> = true>
    L_NODISCARD constexpr auto mul(const vec_type0& a, const vec_type1& b) noexcept;

    template<typename vec_type0, typename vec_type1, std::enable_if_t<is_vector_v<vec_type0> && is_vector_v<vec_type1>, bool> = true>
    L_NODISCARD constexpr auto operator*(const vec_type0& a, const vec_type1& b) noexcept;

    template<typename vec_type, std::enable_if_t<is_vector_v<vec_type>, bool> = true>
    L_NODISCARD constexpr auto mul(const vec_type& a, typename vec_type::scalar b) noexcept;

    template<typename vec_type, std::enable_if_t<is_vector_v<vec_type>, bool> = true>
    L_NODISCARD constexpr auto operator*(const vec_type& a, typename vec_type::scalar b) noexcept;
    
    template<typename vec_type, std::enable_if_t<is_vector_v<vec_type>, bool> = true>
    L_NODISCARD constexpr auto mul(typename vec_type::scalar a, const vec_type& b) noexcept { return mul(b, a); }

    template<typename vec_type, std::enable_if_t<is_vector_v<vec_type>, bool> = true>
    L_NODISCARD constexpr auto operator*(typename vec_type::scalar a, const vec_type& b) noexcept { return mul(b, a); }

    template<typename vec_type0, typename vec_type1, std::enable_if_t<is_vector_v<vec_type0> && is_vector_v<vec_type1>, bool> = true>
    constexpr vec_type0& mul_assign(vec_type0& a, const vec_type1& b) noexcept;

    template<typename vec_type0, typename vec_type1, std::enable_if_t<is_vector_v<vec_type0> && is_vector_v<vec_type1>, bool> = true>
    constexpr vec_type0& operator*=(vec_type0& a, const vec_type1& b) noexcept;

    template<typename vec_type, std::enable_if_t<is_vector_v<vec_type>, bool> = true>
    constexpr vec_type& mul_assign(vec_type& a, typename vec_type::scalar b) noexcept;

    template<typename vec_type, std::enable_if_t<is_vector_v<vec_type>, bool> = true>
    constexpr vec_type& operator*=(vec_type& a, typename vec_type::scalar b) noexcept;

    // Matrix
    template<typename mat_type0, typename mat_type1, std::enable_if_t<is_matrix_v<mat_type0>&& is_matrix_v<mat_type1>, bool> = true>
    L_NODISCARD constexpr auto mul(const mat_type0& a, const mat_type1& b) noexcept;

    template<typename mat_type0, typename mat_type1, std::enable_if_t<is_matrix_v<mat_type0>&& is_matrix_v<mat_type1>, bool> = true>
    L_NODISCARD constexpr auto operator*(const mat_type0& a, const mat_type1& b) noexcept;

    template<typename mat_type, typename vec_type, std::enable_if_t<is_matrix_v<mat_type>&& is_vector_v<vec_type>, bool> = true>
    L_NODISCARD constexpr auto mul(const vec_type& a, const mat_type& b) noexcept;

    template<typename mat_type, typename vec_type, std::enable_if_t<is_matrix_v<mat_type>&& is_vector_v<vec_type>, bool> = true>
    L_NODISCARD constexpr auto operator*(const vec_type& a, const mat_type& b) noexcept;

    template<typename mat_type0, typename mat_type1, std::enable_if_t<is_matrix_v<mat_type0>&& is_matrix_v<mat_type1>, bool> = true>
    constexpr mat_type0& mul_assign(mat_type0& a, const mat_type1& b) noexcept;

    template<typename mat_type0, typename mat_type1, std::enable_if_t<is_matrix_v<mat_type0>&& is_matrix_v<mat_type1>, bool> = true>
    constexpr mat_type0& operator*=(mat_type0& a, const mat_type1& b) noexcept;
}

#include <core/math/arithmetic/multiplication.inl>
