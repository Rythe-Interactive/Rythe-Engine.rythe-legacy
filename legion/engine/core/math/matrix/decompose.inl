#include <core/math/matrix/decompose.hpp>
#pragma once

namespace legion::core::math
{
    template<typename Scalar>
    L_NODISCARD vector<Scalar, 3> scale(const matrix<Scalar, 4, 4>& mat) noexcept
    {
        return { length(mat[0].xyz), length(mat[1].xyz), length(mat[2].xyz) }
    }

    template<typename Scalar>
    L_NODISCARD vector<Scalar, 3> scale(const matrix<Scalar, 3, 3>& mat) noexcept
    {
        return { length(mat[0].xyz), length(mat[1].xyz), length(mat[2].xyz) }
    }

    template<typename Scalar>
    L_NODISCARD quaternion<Scalar> orientation(const matrix<Scalar, 4, 4>& mat) noexcept
    {
        matrix<Scalar, 3, 3> orientationMat;

        for (size_type i = 0; i < 3; i++)
            orientationMat[i] = normalize(mat[i].xyz);

        quaternion<Scalar> result;

        Scalar trace = orientationMat[0].x + orientationMat[1].y + orientationMat[2].z;
        if (trace > static_cast<Scalar>(0))
        {
            Scalar root = ::std::sqrt(trace + static_cast<Scalar>(1));
            result.w = static_cast<Scalar>(0.5) * root;
            root = static_cast<Scalar>(0.5) / root;
            result.x = root * (orientationMat[1].z - orientationMat[2].y);
            result.y = root * (orientationMat[2].x - orientationMat[0].z);
            result.z = root * (orientationMat[0].y - orientationMat[1].x);
        }
        else
        {
            static constexpr size_type nextIdx[3] = { 1, 2, 0 };
            size_type i = 0;

            if (orientationMat[1].y > orientationMat[0].x)
                i = 1;
            if (orientationMat[2].z > orientationMat[i][i])
                i = 2;

            size_type j = nextIdx[i];
            size_type k = nextIdx[j];

            Scalar root = ::std::sqrt(orientationMat[i][i] - orientationMat[j][j] - orientationMat[k][k] + static_cast<Scalar>(1));

            result[i] = static_cast<Scalar>(0.5) * root;
            root = static_cast<Scalar>(0.5) / root;
            result[j] = root * (orientationMat[i][j] + orientationMat[j][i]);
            result[k] = root * (orientationMat[i][k] + orientationMat[k][i]);
            result.w = root * (orientationMat[j][k] - orientationMat[k][j]);
        }

        return result;
    }

    template<typename Scalar>
    L_NODISCARD quaternion<Scalar> orientation(const matrix<Scalar, 3, 3>& mat) noexcept
    {
        matrix<Scalar, 3, 3> orientationMat;

        for (size_type i = 0; i < 3; i++)
            orientationMat[i] = normalize(mat[i].xyz);

        quaternion<Scalar> result;

        Scalar trace = orientationMat[0].x + orientationMat[1].y + orientationMat[2].z;
        if (trace > static_cast<Scalar>(0))
        {
            Scalar root = ::std::sqrt(trace + static_cast<Scalar>(1));
            result.w = static_cast<Scalar>(0.5) * root;
            root = static_cast<Scalar>(0.5) / root;
            result.x = root * (orientationMat[1].z - orientationMat[2].y);
            result.y = root * (orientationMat[2].x - orientationMat[0].z);
            result.z = root * (orientationMat[0].y - orientationMat[1].x);
        }
        else
        {
            static constexpr size_type nextIdx[3] = { 1, 2, 0 };
            size_type i = 0;

            if (orientationMat[1].y > orientationMat[0].x)
                i = 1;
            if (orientationMat[2].z > orientationMat[i][i])
                i = 2;

            size_type j = nextIdx[i];
            size_type k = nextIdx[j];

            Scalar root = ::std::sqrt(orientationMat[i][i] - orientationMat[j][j] - orientationMat[k][k] + static_cast<Scalar>(1));

            result[i] = static_cast<Scalar>(0.5) * root;
            root = static_cast<Scalar>(0.5) / root;
            result[j] = root * (orientationMat[i][j] + orientationMat[j][i]);
            result[k] = root * (orientationMat[i][k] + orientationMat[k][i]);
            result.w = root * (orientationMat[j][k] - orientationMat[k][j]);
        }

        return result;
    }

    template<typename Scalar>
    L_NODISCARD constexpr vector<Scalar, 3> translation(const matrix<Scalar, 4, 4>& mat) noexcept
    {
        if (close_enough(mat[3][3], 1))
        {
            return mat[3].xyz;
        }
        else
        {
            return mat[3].xyz / mat[3].w;
        }
    }

    template<typename Scalar>
    void decompose(const matrix<Scalar, 4, 4>& mat, vector<Scalar, 3>& scale, quaternion<Scalar>& orientation, vector<Scalar, 3>& translation) noexcept
    {
        if (close_enough(mat[3][3], 0))
            return;
        else if (close_enough(mat[3][3], 1))
        {
            scale.x = length(mat[0].xyz);
            scale.y = length(mat[1].xyz);
            scale.z = length(mat[2].xyz);
            translation = mat[3].xyz;

            matrix<Scalar, 3, 3> orientationMat;

            for (size_type i = 0; i < 3; i++)
                orientationMat[i] = mat[i].xyz / scale[i];

            Scalar root;
            Scalar trace = orientationMat[0].x + orientationMat[1].y + orientationMat[2].z;
            if (trace > static_cast<Scalar>(0))
            {
                root = ::std::sqrt(trace + static_cast<Scalar>(1));
                orientation.w = static_cast<Scalar>(0.5) * root;
                root = static_cast<Scalar>(0.5) / root;
                orientation.i = root * (orientationMat[1].z - orientationMat[2].y);
                orientation.j = root * (orientationMat[2].x - orientationMat[0].z);
                orientation.k = root * (orientationMat[0].y - orientationMat[1].x);
            }
            else
            {
                static constexpr size_type nextIdx[3] = { 1, 2, 0 };
                size_type i = 0;

                if (orientationMat[1].y > orientationMat[0].x)
                    i = 1;
                if (orientationMat[2].z > orientationMat[i][i])
                    i = 2;

                size_type j = nextIdx[i];
                size_type k = nextIdx[j];

                root = ::std::sqrt(orientationMat[i][i] - orientationMat[j][j] - orientationMat[k][k] + static_cast<Scalar>(1));

                orientation[i] = static_cast<Scalar>(0.5) * root;
                root = static_cast<Scalar>(0.5) / root;
                orientation[j] = root * (orientationMat[i][j] + orientationMat[j][i]);
                orientation[k] = root * (orientationMat[i][k] + orientationMat[k][i]);
                orientation.w = root * (orientationMat[j][k] - orientationMat[k][j]);
            }
        }
        else
        {
            matrix<Scalar, 4, 4> cpy = mat;

            // Normalize matrix
            Scalar inv = static_cast<Scalar>(1) / cpy[3][3];
            for (size_type i = 0; i < 4; ++i)
                for (size_type j = 0; j < 4; ++j)
                    cpy[i][j] *= inv;

            decompose(cpy, scale, orientation, translation);
        }
    }

    template<typename Scalar>
    void decompose(const matrix<Scalar, 3, 3>& mat, vector<Scalar, 3>& scale, quaternion<Scalar>& orientation) noexcept
    {
        scale.x = length(mat[0].xyz);
        scale.y = length(mat[1].xyz);
        scale.z = length(mat[2].xyz);

        matrix<Scalar, 3, 3> orientationMat;

        for (size_type i = 0; i < 3; i++)
            orientationMat[i] = mat[i].xyz / scale[i];

        Scalar root;
        Scalar trace = orientationMat[0].x + orientationMat[1].y + orientationMat[2].z;
        if (trace > static_cast<Scalar>(0))
        {
            root = ::std::sqrt(trace + static_cast<Scalar>(1));
            orientation.w = static_cast<Scalar>(0.5) * root;
            root = static_cast<Scalar>(0.5) / root;
            orientation.i = root * (orientationMat[1].z - orientationMat[2].y);
            orientation.j = root * (orientationMat[2].x - orientationMat[0].z);
            orientation.k = root * (orientationMat[0].y - orientationMat[1].x);
        }
        else
        {
            static constexpr size_type nextIdx[3] = { 1, 2, 0 };
            size_type i = 0;

            if (orientationMat[1].y > orientationMat[0].x)
                i = 1;
            if (orientationMat[2].z > orientationMat[i][i])
                i = 2;

            size_type j = nextIdx[i];
            size_type k = nextIdx[j];

            root = ::std::sqrt(orientationMat[i][i] - orientationMat[j][j] - orientationMat[k][k] + static_cast<Scalar>(1));

            orientation[i] = static_cast<Scalar>(0.5) * root;
            root = static_cast<Scalar>(0.5) / root;
            orientation[j] = root * (orientationMat[i][j] + orientationMat[j][i]);
            orientation[k] = root * (orientationMat[i][k] + orientationMat[k][i]);
            orientation.w = root * (orientationMat[j][k] - orientationMat[k][j]);
        }
    }

    template<typename Scalar>
    matrix<Scalar, 4, 4> compose(const vector<Scalar, 3>& scale, const quaternion<Scalar>& orientation, const vector<Scalar, 3>& translation) noexcept
    {
        matrix<Scalar, 4, 4> result(orientation);
        result[3].xyz = translation;
        result[0].xyz *= scale[0];
        result[1].xyz *= scale[1];
        result[2].xyz *= scale[2];
        return result;
    }

    template<typename Scalar>
    matrix<Scalar, 3, 3> compose(const vector<Scalar, 3>& scale, const quaternion<Scalar>& orientation) noexcept
    {
        matrix<Scalar, 3, 3> result(orientation);
        result[0] *= scale[0];
        result[1] *= scale[1];
        result[2] *= scale[2];
        return result;
    }
}
