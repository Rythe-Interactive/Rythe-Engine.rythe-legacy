#include <core/math/random/linear_rand.hpp>
#pragma once

namespace legion::core::math
{
    namespace detail
    {
        template<typename T>
        struct compute_linear_rand;

        template<typename Scalar, size_type Size>
        struct compute_linear_rand<vector<Scalar, Size>>
        {
            using vec_type = vector<Scalar, Size>;

            L_NODISCARD static auto compute(const vec_type& min, const vec_type& max)
            {
                vec_type result;
                for (size_type i = 0; i < Size; i++)
                    result[i] = linear_rand(min[i], max[i]);
                return result;
            }


            L_NODISCARD static auto compute(Scalar min, Scalar max)
            {
                vec_type result;
                for (size_type i = 0; i < Size; i++)
                    result[i] = linear_rand(min, max);
                return result;
            }
        };

        template<typename Scalar>
        struct compute_linear_rand<vector<Scalar, 1>>
        {
            L_NODISCARD static auto compute(Scalar min, Scalar max)
            {
                return linear_rand(min, max);
            }
        };
    }

    template<typename Scalar>
    L_NODISCARD Scalar linear_rand(Scalar min, Scalar max)
    {
        if constexpr (::std::is_floating_point_v<Scalar>)
        {
            return min + (max - min) * RandomNumberGenerator::generateRandomFloat<Scalar>();
        }
        else
        {
            static_assert(::std::is_arithmetic_v<Scalar>, "Input scalar type to linear_rand is neither a scalar nor any other supported type.");
            return min + RandomNumberGenerator::generateRandomInteger<Scalar>() % (max - min);
        }
    }

    template<typename vec_type, typename Scalar, ::std::enable_if_t<is_vector_v<vec_type>&& vec_type::size != 1, bool>>
    L_NODISCARD vec_type linear_rand(Scalar min, Scalar max)
    {
        return detail::compute_linear_rand<vec_type>::compute(static_cast<typename vec_type::scalar>(min), static_cast<typename vec_type::scalar>(max));
    }

    template<typename vec_type0, typename vec_type1, ::std::enable_if_t<is_vector_v<vec_type0>&& is_vector_v<vec_type1>&& vec_type0::size == vec_type1::size, bool>>
    L_NODISCARD auto linear_rand(const vec_type0& min, const vec_type1& max)
    {
        using scalar = lowest_precision_t<typename vec_type0::scalar, typename vec_type1::scalar>;
        return detail::compute_linear_rand<vector<scalar, vec_type0::size>>::compute(min, max);
    }
}
