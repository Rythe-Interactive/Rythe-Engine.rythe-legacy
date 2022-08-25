#include <core/math/interpolation/map.hpp>
#pragma once

namespace legion::core::math::detail
{
    template<typename T, typename InMin, typename InMax, typename OutMin, typename OutMax>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto _map_impl_(T&& value, InMin&& inMin, InMax&& inMax, OutMin&& outMin, OutMax&& outMax) noexcept
    {
        return outMin + (outMax - outMin) * (value - inMin) / (inMax - inMin);
    }
}

#include <core/math/interpolation/map_vector.inl>
//#include <core/math/interpolation/lerp_matrix.inl>
//#include <core/math/interpolation/lerp_quaternion.inl>

namespace legion::core::math
{
    // map
    template<typename T, typename InMin, typename InMax, typename OutMin, typename OutMax>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto map(T&& value, InMin&& inMin, InMax&& inMax, OutMin&& outMin, OutMax&& outMax) noexcept
    {
        using ValueType = remove_cvr_t<T>;
        using InMinType = remove_cvr_t<InMin>;
        using InMaxType = remove_cvr_t<InMax>;
        using OutMinType = remove_cvr_t<OutMin>;
        using OutMaxType = remove_cvr_t<OutMax>;

        if constexpr (is_quat_v<ValueType> && is_quat_v<InMinType> && is_quat_v<InMaxType> && is_quat_v<OutMinType> && is_quat_v<OutMaxType>)
        {
            using scalar = elevated_t<typename ValueType::scalar,
                elevated_t<typename InMinType::scalar,
                elevated_t<typename InMaxType::scalar,
                elevated_t<typename OutMinType::scalar, typename OutMaxType::scalar>>>>;
            return detail::compute_map<quaternion<scalar>>::compute(::std::forward<T>(value), ::std::forward<InMin>(inMin), ::std::forward<InMax>(inMax), ::std::forward<OutMin>(outMin), ::std::forward<OutMax>(outMax));
        }
        else if constexpr (is_matrix_v<ValueType> && is_matrix_v<InMinType> && is_matrix_v<InMaxType> && is_matrix_v<OutMinType> && is_matrix_v<OutMaxType>)
        {
            using scalar = elevated_t<typename ValueType::scalar,
                elevated_t<typename InMinType::scalar,
                elevated_t<typename InMaxType::scalar,
                elevated_t<typename OutMinType::scalar, typename OutMaxType::scalar>>>>;
            return detail::compute_map<matrix<scalar, ValueType::row_count, ValueType::col_count>>::compute(::std::forward<T>(value), ::std::forward<InMin>(inMin), ::std::forward<InMax>(inMax), ::std::forward<OutMin>(outMin), ::std::forward<OutMax>(outMax));
        }
        else if constexpr (is_vector_v<ValueType> && is_vector_v<InMinType> && is_vector_v<InMaxType> && is_vector_v<OutMinType> && is_vector_v<OutMaxType>)
        {
            using scalar = elevated_t<typename ValueType::scalar,
                elevated_t<typename InMinType::scalar,
                elevated_t<typename InMaxType::scalar,
                elevated_t<typename OutMinType::scalar, typename OutMaxType::scalar>>>>;
            constexpr size_type size = min(ValueType::size,
                min(InMinType::size,
                min(InMaxType::size,
                min(OutMinType::size, OutMaxType::size))));
            return detail::compute_map<vector<scalar, size>>::compute(::std::forward<T>(value), ::std::forward<InMin>(inMin), ::std::forward<InMax>(inMax), ::std::forward<OutMin>(outMin), ::std::forward<OutMax>(outMax));
        }
        else
        {
            return detail::_map_impl_(::std::forward<T>(value), ::std::forward<InMin>(inMin), ::std::forward<InMax>(inMax), ::std::forward<OutMin>(outMin), ::std::forward<OutMax>(outMax));
        }
    }

    // map01
    template<typename T, typename InMin, typename InMax>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto map01(T&& value, InMin&& inMin, InMax&& inMax) noexcept
    {
        return map(::std::forward<T>(value), ::std::forward<InMin>(inMin), ::std::forward<InMax>(inMax), zero<T>, one<T>);
    }
}
