#pragma once
#include <core/math/interpolation/map.hpp>

namespace legion::core::math::detail
{
    template<typename T>
    struct compute_cos;

    template<typename Scalar, size_type Size>
    struct compute_cos<vector<Scalar, Size>>
    {
        using vec_type = vector<Scalar, Size>;

        template<typename ValueType>
        L_NODISCARD static auto compute(ValueType&& value) noexcept
        {
            vec_type result;
            for (size_type i = 0; i < Size; i++)
                result[i] = ::std::cos(value[i]);

            return result;
        }

        template<typename ValueType>
        L_NODISCARD static auto compute_inverse(ValueType&& value) noexcept
        {
            vec_type result;
            for (size_type i = 0; i < Size; i++)
                result[i] = ::std::acos(value[i]);

            return result;
        }
    };

    template<typename Scalar>
    struct compute_cos<vector<Scalar, 1u>>
    {
        using vec_type = vector<Scalar, 1u>;

        template<typename ValueType>
        L_NODISCARD static auto compute(ValueType&& value) noexcept
        {
            return ::std::cos(value[0]);
        }

        template<typename ValueType>
        L_NODISCARD static auto compute_inverse(ValueType&& value) noexcept
        {
            return ::std::acos(value[0]);
        }
    };
}
