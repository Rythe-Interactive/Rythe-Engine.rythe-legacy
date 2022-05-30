#pragma once
#include <core/types/primitives.hpp>

#include <core/math/matrix/matrix4.hpp>
#include <core/math/quaternion/quaternion.hpp>
#include <core/math/vector/vector.hpp>

#include <core/math/geometric/length.hpp>
#include <core/math/geometric/normalize.hpp>

#include <core/math/util/meta.hpp>
#include <core/math/util/close_enough.hpp>

namespace legion::core::math
{
    template<typename Scalar>
    L_NODISCARD vector<Scalar, 3> scale(const matrix<Scalar, 4, 4>& mat) noexcept;

    template<typename Scalar>
    L_NODISCARD vector<Scalar, 3> scale(const matrix<Scalar, 3, 3>& mat) noexcept;

    template<typename Scalar>
    L_NODISCARD quaternion<Scalar> orientation(const matrix<Scalar, 4, 4>& mat) noexcept;

    template<typename Scalar>
    L_NODISCARD quaternion<Scalar> orientation(const matrix<Scalar, 3, 3>& mat) noexcept;

    template<typename Scalar>
    L_NODISCARD constexpr vector<Scalar, 3> translation(const matrix<Scalar, 4, 4>& mat) noexcept;

    template<typename Scalar>
    void decompose(const matrix<Scalar, 4, 4>& mat, vector<Scalar, 3>& scale, quaternion<Scalar>& orientation, vector<Scalar, 3>& translation) noexcept;

    template<typename Scalar>
    void decompose(const matrix<Scalar, 3, 3>& mat, vector<Scalar, 3>& scale, quaternion<Scalar>& orientation) noexcept;

    template<typename Scalar>
    matrix<Scalar, 4, 4> compose(const vector<Scalar, 3>& scale, const quaternion<Scalar>& orientation, const vector<Scalar, 3>& translation) noexcept;

    template<typename Scalar>
    matrix<Scalar, 3, 3> compose(const vector<Scalar, 3>& scale, const quaternion<Scalar>& orientation) noexcept;
}

#include <core/math/matrix/decompose.inl>
