#pragma once
#include <core/math/geometric/normalize.hpp>

namespace legion::core::math::detail
{
    template<typename T>
    struct compute_normalize;

    template<typename Scalar>
    struct compute_normalize<quaternion<Scalar>>
    {
        using quat_type = quaternion<Scalar>;

        L_NODISCARD L_ALWAYS_INLINE static quat_type compute(const quat_type& q) noexcept
        {
            return q / length(q);
        }
    };
}
