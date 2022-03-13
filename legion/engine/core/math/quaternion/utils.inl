#pragma once
#include <cmath>

#include <core/math/quaternion/quaternion_base.hpp>
#include <core/math/geometric/normalize.hpp>
#include <core/math/geometric/cross.hpp>
#include <core/math/arithmetic/multiplication.hpp>

namespace legion::core::math
{
    template<typename Scalar>
    L_NODISCARD L_ALWAYS_INLINE vector<Scalar, 3> quaternion<Scalar>::right() noexcept
    {
        return vec_type::right * *this;
    }

    template<typename Scalar>
    L_NODISCARD L_ALWAYS_INLINE vector<Scalar, 3> quaternion<Scalar>::forward() noexcept
    {
        return vec_type::forward * *this;
    }

    template<typename Scalar>
    L_NODISCARD L_ALWAYS_INLINE vector<Scalar, 3> quaternion<Scalar>::up() noexcept
    {
        return vec_type::up * *this;
    }

    template<typename Scalar>
    L_NODISCARD L_ALWAYS_INLINE quaternion<Scalar> quaternion<Scalar>::angle_axis(scalar angle, const vec_type& vec) noexcept
    {
        auto halfAngle = angle * static_cast<scalar>(0.5);
        return quaternion<scalar>(::std::cos(halfAngle), ::std::sin(halfAngle) * vec);
    }

    template<typename Scalar>
    L_NODISCARD L_ALWAYS_INLINE quaternion<Scalar> quaternion<Scalar>::look_at(const vec_type& _pos, const vec_type& _center, const vec_type& _up) noexcept
    {
        vec_type const forward(normalize(_center - _pos));
        vec_type const right(normalize(cross(_up, forward)));
        vec_type const up(cross(forward, right));

        const scalar qwijk[] = {
            right.x - up.y - forward.z,
            up.y - right.x - forward.z,
            forward.z - right.x - up.y,
            right.x + up.y + forward.z };

        size_type idx = 0;
        scalar qMax = qwijk[0];
        for (size_type i = 1; i < 4; i++)
            if (qwijk[i] > qMax)
            {
                qMax = qwijk[i];
                idx = i;
            }

        qMax = ::std::sqrt(qMax + static_cast<scalar>(1)) * static_cast<scalar>(0.5);
        scalar mult = static_cast<scalar>(0.25) / qMax;

        scalar qPerms[] = {
            qMax,
            (forward.y - up.z) * mult,
            (right.z - forward.x) * mult,
            (up.x - right.y) * mult };

        size_type invIdx = 3 - idx;
        return quaternion(
            qPerms[idx],
            qPerms[(invIdx + 2) % 4],
            qPerms[(idx + 2) % 4],
            qPerms[invIdx]);
    }
}
