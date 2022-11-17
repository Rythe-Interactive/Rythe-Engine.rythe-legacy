#include <core/math/random/circular_rand.hpp>
#pragma once

namespace legion::core::math
{
    template<typename Scalar>
    L_NODISCARD vector<Scalar, 2> circular_rand(Scalar radius) noexcept
    {
        static_assert(::std::is_arithmetic_v<Scalar>, "Input scalar type to circular_rand is neither a scalar nor any other supported type.");

        Scalar theta = linear_rand(static_cast<Scalar>(0), tau<Scalar>());
        return vector<Scalar, 2>(cos(theta), sin(theta)) * radius;
    }

    template<typename vec_type, ::std::enable_if_t<is_vector_v<vec_type>&& vec_type::size == 2, bool>>
    L_NODISCARD auto circular_rand(const vec_type& radii) noexcept
    {
        using Scalar = typename vec_type::scalar;

        Scalar theta = linear_rand(static_cast<Scalar>(0), tau<Scalar>());
        return vector<Scalar, 2>(cos(theta), sin(theta)) * radii;
    }
}
