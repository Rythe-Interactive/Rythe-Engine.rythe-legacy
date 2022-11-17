#pragma once
#include <core/math/interpolation/map.hpp>

namespace legion::core::math::detail
{
    template<typename T>
    struct compute_map;

    template<typename Scalar, size_type Size>
    struct compute_map<vector<Scalar, Size>>
    {
        using vec_type = vector<Scalar, Size>;

        template<typename T, typename InMin, typename InMax, typename OutMin, typename OutMax>
        L_NODISCARD constexpr static auto compute(T&& value, InMin&& inMin, InMax&& inMax, OutMin&& outMin, OutMax&& outMax) noexcept
        {
            vec_type result;
            for (size_type i = 0; i < Size; i++)
                result[i] = detail::_map_impl_(_extract_item_(value, i), _extract_item_(inMin, i), _extract_item_(inMax, i), _extract_item_(outMin, i), _extract_item_(outMax, i));

            return result;
        }
    };

    template<typename Scalar>
    struct compute_map<vector<Scalar, 1u>>
    {
        using vec_type = vector<Scalar, 1u>;

        template<typename T, typename InMin, typename InMax, typename OutMin, typename OutMax>
        L_NODISCARD constexpr static Scalar compute(T&& value, InMin&& inMin, InMax&& inMax, OutMin&& outMin, OutMax&& outMax) noexcept
        {
            return detail::_map_impl_(_extract_item_(value, 0), _extract_item_(inMin, 0), _extract_item_(inMax, 0), _extract_item_(outMin, 0), _extract_item_(outMax, 0));
        }
    };
}
