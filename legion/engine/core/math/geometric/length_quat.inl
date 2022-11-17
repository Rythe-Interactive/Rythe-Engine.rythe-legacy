#pragma once
#include <core/math/geometric/length.hpp>

namespace legion::core::math::detail
{
    template<typename T>
    struct compute_length;

    template<typename Scalar>
    struct compute_length<quaternion<Scalar>>
    {
        using quat_type = quaternion<Scalar>;
        using vec_type = vector<Scalar, 4>;

        L_NODISCARD L_ALWAYS_INLINE static Scalar compute(const quat_type& q) noexcept
        {
            vec_type v(q.w, q.i, q.j, q.k);
            return ::std::sqrt(dot(v, v));
        }

        L_NODISCARD L_ALWAYS_INLINE constexpr static Scalar compute2(const quat_type& q) noexcept
        {
            vec_type v(q.w, q.i, q.j, q.k);
            return dot(v, v);
        }
    };
}
