#pragma once
#include <core/math/matrix/matrix2.hpp>
#include <core/math/matrix/matrix3.hpp>
#include <core/math/matrix/matrix4.hpp>
#include <core/math/quaternion/quaternion.hpp>

namespace legion::core::math
{
    template<typename Scalar>
    template<typename Scal>
    constexpr matrix<Scalar, 2, 2>::matrix(const quaternion<Scal>& orientation) noexcept
    {
        Scalar i2 = static_cast<Scalar>(orientation.i * orientation.i);
        Scalar j2 = static_cast<Scalar>(orientation.j * orientation.j);
        Scalar k2 = static_cast<Scalar>(orientation.k * orientation.k);
        Scalar ij = static_cast<Scalar>(orientation.i * orientation.j);
        Scalar wk = static_cast<Scalar>(orientation.w * orientation.k);

        rows[0][0] = static_cast<Scalar>(1) - static_cast<Scalar>(2) * (j2 + k2);
        rows[0][1] = static_cast<Scalar>(2) * (ij + wk);

        rows[1][0] = static_cast<Scalar>(2) * (ij - wk);
        rows[1][1] = static_cast<Scalar>(1) - static_cast<Scalar>(2) * (i2 + k2);
    }

    template<typename Scalar>
    template<typename Scal>
    constexpr matrix<Scalar, 3, 3>::matrix(const quaternion<Scal>& orientation) noexcept
    {
        Scalar i2 = static_cast<Scalar>(orientation.i * orientation.i);
        Scalar j2 = static_cast<Scalar>(orientation.j * orientation.j);
        Scalar k2 = static_cast<Scalar>(orientation.k * orientation.k);
        Scalar ik = static_cast<Scalar>(orientation.i * orientation.k);
        Scalar ij = static_cast<Scalar>(orientation.i * orientation.j);
        Scalar jk = static_cast<Scalar>(orientation.j * orientation.k);
        Scalar wi = static_cast<Scalar>(orientation.w * orientation.i);
        Scalar wj = static_cast<Scalar>(orientation.w * orientation.j);
        Scalar wk = static_cast<Scalar>(orientation.w * orientation.k);

        rows[0][0] = static_cast<Scalar>(1) - static_cast<Scalar>(2) * (j2 + k2);
        rows[0][1] = static_cast<Scalar>(2) * (ij + wk);
        rows[0][2] = static_cast<Scalar>(2) * (ik - wj);

        rows[1][0] = static_cast<Scalar>(2) * (ij - wk);
        rows[1][1] = static_cast<Scalar>(1) - static_cast<Scalar>(2) * (i2 + k2);
        rows[1][2] = static_cast<Scalar>(2) * (jk + wi);

        rows[2][0] = static_cast<Scalar>(2) * (ik + wj);
        rows[2][1] = static_cast<Scalar>(2) * (jk - wi);
        rows[2][2] = static_cast<Scalar>(1) - static_cast<Scalar>(2) * (i2 + j2);
    }

    template<typename Scalar>
    template<typename Scal>
    constexpr matrix<Scalar, 4, 4>::matrix(const quaternion<Scal>& orientation) noexcept
        : row3(float4::positiveW)
    {
        Scalar i2 = static_cast<Scalar>(orientation.i * orientation.i);
        Scalar j2 = static_cast<Scalar>(orientation.j * orientation.j);
        Scalar k2 = static_cast<Scalar>(orientation.k * orientation.k);
        Scalar ik = static_cast<Scalar>(orientation.i * orientation.k);
        Scalar ij = static_cast<Scalar>(orientation.i * orientation.j);
        Scalar jk = static_cast<Scalar>(orientation.j * orientation.k);
        Scalar wi = static_cast<Scalar>(orientation.w * orientation.i);
        Scalar wj = static_cast<Scalar>(orientation.w * orientation.j);
        Scalar wk = static_cast<Scalar>(orientation.w * orientation.k);

        rows[0][0] = static_cast<Scalar>(1) - static_cast<Scalar>(2) * (j2 + k2);
        rows[0][1] = static_cast<Scalar>(2) * (ij + wk);
        rows[0][2] = static_cast<Scalar>(2) * (ik - wj);
        rows[0][3] = static_cast<Scalar>(0);

        rows[1][0] = static_cast<Scalar>(2) * (ij - wk);
        rows[1][1] = static_cast<Scalar>(1) - static_cast<Scalar>(2) * (i2 + k2);
        rows[1][2] = static_cast<Scalar>(2) * (jk + wi);
        rows[1][3] = static_cast<Scalar>(0);

        rows[2][0] = static_cast<Scalar>(2) * (ik + wj);
        rows[2][1] = static_cast<Scalar>(2) * (jk - wi);
        rows[2][2] = static_cast<Scalar>(1) - static_cast<Scalar>(2) * (i2 + j2);
        rows[2][3] = static_cast<Scalar>(0);
    }
}
