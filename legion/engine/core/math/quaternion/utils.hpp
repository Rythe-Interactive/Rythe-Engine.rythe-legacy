#pragma once
#include <cmath>

#include <core/math/quaternion/quaternion_base.hpp>

namespace legion::core::math
{
    template<typename T>
    L_NODISCARD L_ALWAYS_INLINE static auto angle_axis(T angle, const vector<remove_cvr_t<T>, 3>& vec)
    {
        using scalar = remove_cvr_t<T>;
        auto halfAngle = angle * static_cast<scalar>(0.5);
        return quaternion<scalar>(::std::cos(halfAngle), ::std::sin(halfAngle) * vec);
    }
}
