#pragma once
#include <core/math/interpolation/map.hpp>

namespace legion::core::math::detail
{
    template<typename T>
    struct compute_sin;

    template<typename Scalar, size_type Size>
    struct compute_sin<vector<Scalar, Size>>
    {
        using vec_type = vector<Scalar, Size>;

        template<typename ValueType>
        L_NODISCARD static auto compute(ValueType&& value) noexcept
        {
            vec_type result;
            for (size_type i = 0; i < Size; i++)
                result[i] = ::std::sin(value[i]);

            return result;
        }

        template<typename ValueType>
        L_NODISCARD static auto compute_inverse(ValueType&& value) noexcept
        {
            vec_type result;
            for (size_type i = 0; i < Size; i++)
                result[i] = ::std::asin(value[i]);

            return result;
        }
    };

    template<typename Scalar>
    struct compute_sin<vector<Scalar, 1u>>
    {
        using vec_type = vector<Scalar, 1u>;

        template<typename ValueType>
        L_NODISCARD static auto compute(ValueType&& value) noexcept
        {
            return ::std::sin(value[0]);
        }

        template<typename ValueType>
        L_NODISCARD static auto compute_inverse(ValueType&& value) noexcept
        {
            return ::std::asin(value[0]);
        }
    };
}
