#include <core/math/vector/vector3.hpp>
#pragma once

namespace legion::core::math
{
    template<typename Scalar>
    inline L_ALWAYS_INLINE constexpr vector<Scalar, 3>::vector(const vector<scalar, 2>& v, scalar s) noexcept : xyz(v.x, v.y, s) {}

    template<typename Scalar>
    inline L_ALWAYS_INLINE constexpr vector<Scalar, 3>::vector(scalar s, const vector<scalar, 2>& v) noexcept : xyz(s, v.x, v.y) {}

    template<typename Scalar>
    inline L_ALWAYS_INLINE constexpr vector<Scalar, 3>::vector(scalar _x, scalar _y, scalar _z) noexcept : xyz(_x, _y, _z) {}

    template<typename Scalar>
    template<typename vec_type, ::std::enable_if_t<is_vector_v<vec_type> && (vec_type::size != 3 || !std::is_same_v<Scalar, typename vec_type::scalar>), bool>>
    inline L_ALWAYS_INLINE constexpr vector<Scalar, 3>::vector(const vec_type& other) noexcept
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
