#pragma once
#include <core/math/arithmetic/multiplication.hpp>
#include <core/math/geometric/dot.hpp>
#include <core/math/geometric/cross.hpp>

namespace legion::core::math::detail
{
    template<typename T>
    struct compute_multiplication;

    template<typename Scalar>
    struct compute_multiplication<quaternion<Scalar>>
    {
        using quat_type = quaternion<Scalar>;

        template<typename other_type, ::std::enable_if_t<is_quat_v<other_type>, bool> = true>
        L_NODISCARD constexpr static auto compute(const quat_type& a, const other_type& b) noexcept
        {
            quat_type result;
            result.vec = a.w * b.vec + static_cast<Scalar>(b.w) * a.vec + cross(a.vec, b.vec);
            result.w = a.w * static_cast<Scalar>(b.w) - dot(a.vec, b.vec);
            return result;
        }

        template<typename vec_type, ::std::enable_if_t<is_vector_v<vec_type>, bool> = true>
        L_NODISCARD constexpr static auto compute(const vec_type& a, const quat_type& b) noexcept
        {
            using scalar = typename vec_type::scalar;
            if constexpr (vec_type::size == 3)
            {
                return
                    static_cast<scalar>(2) * dot(b.vec, a) * b.vec
                    + (static_cast<scalar>(b.w) * static_cast<scalar>(b.w) - dot(b.vec, b.vec)) * a
                    + static_cast<scalar>(2) * static_cast<scalar>(b.w) * cross(b.vec, a);
            }
            else
            {
                vector<scalar, 3> temp =
                    static_cast<scalar>(2) * dot(b.vec, a.xyz) * b.vec
                    + (static_cast<scalar>(b.w) * static_cast<scalar>(b.w) - dot(b.vec, b.vec)) * a.xyz
                    + static_cast<scalar>(2) * static_cast<scalar>(b.w) * cross(b.vec, a.xyz);

                vec_type result;
                result[0] = temp[0];
                if constexpr (vec_type::size > 1)
                    result[1] = temp[1];
                if constexpr (vec_type::size > 2)
                    result[2] = temp[2];
                return result;
            }
        }
    };
}
