#pragma once
#include <core/math/geometric/inverse.hpp>

namespace legion::core::math::detail
{
    template<typename T>
    struct compute_inverse;

    template<typename Scalar>
    struct compute_inverse<quaternion<Scalar>>
    {
        using quat_type = quaternion<Scalar>;

        L_NODISCARD L_ALWAYS_INLINE static quat_type compute(const quat_type& q) noexcept
        {
            // Assuming we never work with non-normalized quaternions.
            return quat_type(q.w, -q.vec);
        }
    };
}
