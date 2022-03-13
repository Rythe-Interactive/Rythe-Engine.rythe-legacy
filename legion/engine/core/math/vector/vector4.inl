#include <core/math/vector/vector4.hpp>
#pragma once

namespace legion::core::math
{
    template<typename Scalar>
    inline L_ALWAYS_INLINE constexpr vector<Scalar, 4>::vector(const vector<scalar, 3>& v, scalar s) noexcept : xyzw(v.x, v.y, v.z, s) {}

    template<typename Scalar>
    inline L_ALWAYS_INLINE constexpr vector<Scalar, 4>::vector(scalar s, const vector<scalar, 3>& v) noexcept : xyzw(s, v.x, v.y, v.z) {}

    template<typename Scalar>
    inline L_ALWAYS_INLINE constexpr vector<Scalar, 4>::vector(scalar s0, const vector<scalar, 2>& v, scalar s1) noexcept : xyzw(s0, v.x, v.y, s1) {}

    template<typename Scalar>
    inline L_ALWAYS_INLINE constexpr vector<Scalar, 4>::vector(scalar s0, scalar s1, const vector<scalar, 2>& v) noexcept : xyzw(s0, s1, v.x, v.y) {}

    template<typename Scalar>
    inline L_ALWAYS_INLINE constexpr vector<Scalar, 4>::vector(const vector<scalar, 2>& v, scalar s0, scalar s1) noexcept : xyzw(v.x, v.y, s0, s1) {}

    template<typename Scalar>
    inline L_ALWAYS_INLINE constexpr vector<Scalar, 4>::vector(const vector<scalar, 2>& v0, const vector<scalar, 2>& v1) noexcept : xyzw(v0.x, v0.y, v1.x, v1.y) {}

    template<typename Scalar>
    inline L_ALWAYS_INLINE constexpr vector<Scalar, 4>::vector(scalar _x, scalar _y, scalar _z, scalar _w) noexcept : xyzw(_x, _y, _z, _w) {}

    template<typename Scalar>
    template<typename vec_type, ::std::enable_if_t<is_vector_v<vec_type> && (vec_type::size != 4 || !std::is_same_v<Scalar, typename vec_type::scalar>), bool>>
    inline L_ALWAYS_INLINE constexpr vector<Scalar, 4>::vector(const vec_type& other) noexcept
    {
        if constexpr (size > vec_type::size)
        {
            for (size_type i = 0; i < vec_type::size; i++)
                data[i] = static_cast<scalar>(other.data[i]);

            for (size_type i = vec_type::size; i < size; i++)
                data[i] = static_cast<scalar>(0);
        }
        else
        {
            for (size_type i = 0; i < size; i++)
                data[i] = static_cast<scalar>(other.data[i]);
        }
    }
}
