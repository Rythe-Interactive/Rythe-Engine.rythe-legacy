#pragma once
#include <core/math/vector/vector.hpp>
#include <core/math/meta.hpp>

#include <core/math/random/randomnumbergenerator.hpp>

namespace legion::core::math
{
    template<typename Scalar>
    L_NODISCARD Scalar linear_rand(Scalar min, Scalar max);

    template<typename vec_type, typename Scalar, ::std::enable_if_t<is_vector_v<vec_type> && vec_type::size != 1, bool> = true>
    L_NODISCARD vec_type linear_rand(Scalar min, Scalar max);

    template<typename vec_type0, typename vec_type1, ::std::enable_if_t<is_vector_v<vec_type0> && is_vector_v<vec_type1> && vec_type0::size == vec_type1::size, bool> = true>
    L_NODISCARD auto linear_rand(const vec_type0& min, const vec_type1& max);
}

#include <core/math/random/linear_rand.inl>
