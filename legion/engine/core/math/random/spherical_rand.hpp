#pragma once
#include <core/math/random/linear_rand.hpp>
#include <core/math/constants.hpp>

namespace legion::core::math
{
    template<typename Scalar>
    L_NODISCARD vector<Scalar, 3> spherical_rand(Scalar radius) noexcept;

    template<typename vec_type, ::std::enable_if_t<is_vector_v<vec_type>&& vec_type::size == 3, bool> = true>
    L_NODISCARD auto spherical_rand(const vec_type& radii) noexcept;
}

#include <core/math/random/spherical_rand.inl>
