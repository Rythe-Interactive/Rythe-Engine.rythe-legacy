#include <core/math/random/spherical_rand.hpp>
#pragma once

namespace legion::core::math
{
    template<typename Scalar>
    L_NODISCARD vector<Scalar, 3> spherical_rand(Scalar radius) noexcept
    {
        static_assert(::std::is_arithmetic_v<Scalar>, "Input scalar type to spherical_rand is neither a scalar nor any other supported type.");

        Scalar theta = linear_rand(static_cast<Scalar>(0), tau<Scalar>());
        Scalar z = linear_rand(static_cast<Scalar>(-1), static_cast<Scalar>(1));
        Scalar phi = acos(z);
        Scalar x = sin(phi) * cos(theta);
        Scalar y = sin(phi) * sin(theta);

        return vector<3, Scalar>(x, y, z) * radius;
    }

    template<typename vec_type, ::std::enable_if_t<is_vector_v<vec_type>&& vec_type::size == 3, bool>>
    L_NODISCARD auto spherical_rand(const vec_type& radii) noexcept
    {
        using Scalar = typename vec_type::scalar;

        Scalar theta = linear_rand(static_cast<Scalar>(0), tau<Scalar>());
        Scalar z = linear_rand(static_cast<Scalar>(-1), static_cast<Scalar>(1));
        Scalar phi = acos(z);
        Scalar x = sin(phi) * cos(theta);
        Scalar y = sin(phi) * sin(theta);
        if constexpr (alignof(vec_type) == alignof(aligned_vector3<Scalar>))
            return aligned_vector3<Scalar>(x, y, z) * radii;
        else
            return vector<Scalar, 3>(x, y, z) * radii;
    }
}
