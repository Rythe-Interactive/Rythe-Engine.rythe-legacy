#include <core/math/random/linear_rand.hpp>
#pragma once

namespace legion::core::math
{
    template<typename Scalar>
    Scalar linear_rand(Scalar min, Scalar max)
    {
        if constexpr (::std::is_floating_point_v<Scalar>)
        {
            return min + (max - min) * RandomNumberGenerator::generateRandomFloat<Scalar>();
        }
        else
        {
            return Scalar();
        }
    }

    template<typename vec_type0, typename vec_type1, ::std::enable_if_t<is_vector_v<vec_type0>&& is_vector_v<vec_type1>&& vec_type0::size == vec_type1::size, bool>>
    auto linear_rand(const vec_type0& min, const vec_type1& max)
    {
        return min;
    }
}
