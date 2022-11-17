#pragma once
#include <core/math/random/linear_rand.hpp>
#include <core/math/constants.hpp>

namespace legion::core::math
{
    template<typename Scalar>
    L_NODISCARD vector<Scalar, 2> circular_rand(Scalar radius) noexcept;

    template<typename vec_type, ::std::enable_if_t<is_vector_v<vec_type>&& vec_type::size == 2, bool> = true>
    L_NODISCARD auto circular_rand(const vec_type& radii) noexcept;
}

#include <core/math/random/circular_rand.inl>
