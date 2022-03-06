#pragma once
#include <core/types/primitives.hpp>
#include <core/math/vector/vector.hpp>
#include <core/math/matrix/matrix.hpp>
#include <core/math/quaternion/quaternion.hpp>
#include <core/math/meta.hpp>

namespace legion::core::math
{
    // mul
    template<typename vec_type0, typename vec_type1, std::enable_if_t<is_vector_v<vec_type0>&& is_vector_v<vec_type1>, bool> = true>
    L_NODISCARD constexpr auto mul(const vec_type0& a, const vec_type1& b) noexcept;

    template<typename vec_type, std::enable_if_t<is_vector_v<vec_type>, bool> = true>
    L_NODISCARD constexpr auto mul(const vec_type& a, typename vec_type::scalar b) noexcept;

    template<typename vec_type, std::enable_if_t<is_vector_v<vec_type>, bool> = true>
    L_NODISCARD constexpr auto mul(typename vec_type::scalar a, const vec_type& b) noexcept { return mul(b, a); }

    template<typename mat_type0, typename mat_type1, std::enable_if_t<is_matrix_v<mat_type0>&& is_matrix_v<mat_type1>, bool> = true>
    L_NODISCARD constexpr auto mul(const mat_type0& a, const mat_type1& b) noexcept;

    template<typename mat_type, typename vec_type, std::enable_if_t<is_matrix_v<mat_type>&& is_vector_v<vec_type>, bool> = true>
    L_NODISCARD constexpr auto mul(const vec_type& a, const mat_type& b) noexcept;

    template<typename quat_type0, typename quat_type1, std::enable_if_t<is_quat_v<quat_type0>&& is_quat_v<quat_type1>, bool> = true>
    L_NODISCARD constexpr auto mul(const quat_type0& a, const quat_type1& b) noexcept;

    template<typename quat_type, typename vec_type, std::enable_if_t<is_quat_v<quat_type>&& is_vector_v<vec_type>, bool> = true>
    L_NODISCARD constexpr auto mul(const vec_type& a, const quat_type& b) noexcept;


    // operator *
    template<typename vec_type0, typename vec_type1, std::enable_if_t<is_vector_v<vec_type0>&& is_vector_v<vec_type1>, bool> = true>
    L_NODISCARD constexpr auto operator*(const vec_type0& a, const vec_type1& b) noexcept;

    template<typename vec_type, std::enable_if_t<is_vector_v<vec_type>, bool> = true>
    L_NODISCARD constexpr auto operator*(const vec_type& a, typename vec_type::scalar b) noexcept;

    template<typename vec_type, std::enable_if_t<is_vector_v<vec_type>, bool> = true>
    L_NODISCARD constexpr auto operator*(typename vec_type::scalar a, const vec_type& b) noexcept { return mul(b, a); }

    template<typename mat_type0, typename mat_type1, std::enable_if_t<is_matrix_v<mat_type0>&& is_matrix_v<mat_type1>, bool> = true>
    L_NODISCARD constexpr auto operator*(const mat_type0& a, const mat_type1& b) noexcept;

    template<typename mat_type, typename vec_type, std::enable_if_t<is_matrix_v<mat_type>&& is_vector_v<vec_type>, bool> = true>
    L_NODISCARD constexpr auto operator*(const vec_type& a, const mat_type& b) noexcept;

    template<typename quat_type0, typename quat_type1, std::enable_if_t<is_quat_v<quat_type0>&& is_quat_v<quat_type1>, bool> = true>
    L_NODISCARD constexpr auto operator*(const quat_type0& a, const quat_type1& b) noexcept;

    template<typename quat_type, typename vec_type, std::enable_if_t<is_quat_v<quat_type>&& is_vector_v<vec_type>, bool> = true>
    L_NODISCARD constexpr auto operator*(const vec_type& a, const quat_type& b) noexcept;


    // mul_assign
    template<typename vec_type0, typename vec_type1, std::enable_if_t<is_vector_v<vec_type0>&& is_vector_v<vec_type1>, bool> = true>
    constexpr vec_type0& mul_assign(vec_type0& a, const vec_type1& b) noexcept;

    template<typename vec_type, std::enable_if_t<is_vector_v<vec_type>, bool> = true>
    constexpr vec_type& mul_assign(vec_type& a, typename vec_type::scalar b) noexcept;

    template<typename mat_type0, typename mat_type1, std::enable_if_t<is_matrix_v<mat_type0>&& is_matrix_v<mat_type1>, bool> = true>
    constexpr mat_type0& mul_assign(mat_type0& a, const mat_type1& b) noexcept;

    template<typename mat_type, typename vec_type, std::enable_if_t<is_matrix_v<mat_type>&& is_vector_v<vec_type>, bool> = true>
    constexpr vec_type& mul_assign(vec_type& a, const mat_type& b) noexcept;

    template<typename quat_type0, typename quat_type1, std::enable_if_t<is_quat_v<quat_type0>&& is_quat_v<quat_type1>, bool> = true>
    constexpr quat_type0& mul_assign(quat_type0& a, const quat_type1& b) noexcept;

    template<typename quat_type, typename vec_type, std::enable_if_t<is_quat_v<quat_type>&& is_vector_v<vec_type>, bool> = true>
    constexpr vec_type& mul_assign(vec_type& a, const quat_type& b) noexcept;


    // operator *=
    template<typename vec_type0, typename vec_type1, std::enable_if_t<is_vector_v<vec_type0>&& is_vector_v<vec_type1>, bool> = true>
    constexpr vec_type0& operator*=(vec_type0& a, const vec_type1& b) noexcept;

    template<typename vec_type, std::enable_if_t<is_vector_v<vec_type>, bool> = true>
    constexpr vec_type& operator*=(vec_type& a, typename vec_type::scalar b) noexcept;

    template<typename mat_type0, typename mat_type1, std::enable_if_t<is_matrix_v<mat_type0>&& is_matrix_v<mat_type1>, bool> = true>
    constexpr mat_type0& operator*=(mat_type0& a, const mat_type1& b) noexcept;

    template<typename mat_type, typename vec_type, std::enable_if_t<is_matrix_v<mat_type>&& is_vector_v<vec_type>, bool> = true>
    constexpr vec_type& operator*=(vec_type& a, const mat_type& b) noexcept;

    template<typename quat_type0, typename quat_type1, std::enable_if_t<is_quat_v<quat_type0>&& is_quat_v<quat_type1>, bool> = true>
    constexpr quat_type0& operator*=(quat_type0& a, const quat_type1& b) noexcept;

    template<typename quat_type, typename vec_type, std::enable_if_t<is_quat_v<quat_type>&& is_vector_v<vec_type>, bool> = true>
    constexpr vec_type& operator*=(vec_type& a, const quat_type& b) noexcept;
}

#include <core/math/arithmetic/multiplication.inl>
