#include <core/math/basic/constraint.hpp>
#pragma once

#include <core/math/basic/constraint_vector.inl>

namespace legion::core::math
{
    template<typename A, typename B>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto min(A&& a, B&& b)
    {
        if constexpr (is_vector_v<A> && is_vector_v<B>)
        {
            using AType = remove_cvr_t<A>;
            using BType = remove_cvr_t<B>;
            using scalar = elevated_t<typename AType::scalar, typename BType::scalar>;
            constexpr size_type size = AType::size < BType::size ? AType::size : BType::size;
            return detail::compute_constraint<vector<scalar, size>>::compute_min(std::forward<A>(a), std::forward<B>(b));
        }
        else if constexpr (is_vector_v<A> || is_vector_v<B>)
        {
            return detail::compute_constraint<vector_type_t<A, B>>::compute_min(std::forward<A>(a), std::forward<B>(b));
        }
        else
        {
            using value_type = elevated_t<A, B>;
            return static_cast<value_type>(a) < static_cast<value_type>(b) ? static_cast<value_type>(a) : static_cast<value_type>(b);
        }
    }

    template<typename A, typename B>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto max(A&& a, B&& b)
    {
        if constexpr (is_vector_v<A> && is_vector_v<B>)
        {
            using AType = remove_cvr_t<A>;
            using BType = remove_cvr_t<B>;
            using scalar = elevated_t<typename AType::scalar, typename BType::scalar>;
            constexpr size_type size = AType::size < BType::size ? AType::size : BType::size;
            return detail::compute_constraint<vector<scalar, size>>::compute_max(std::forward<A>(a), std::forward<B>(b));
        }
        else if constexpr (is_vector_v<A> || is_vector_v<B>)
        {
            return detail::compute_constraint<vector_type_t<A, B>>::compute_max(std::forward<A>(a), std::forward<B>(b));
        }
        else
        {
            using value_type = elevated_t<A, B>;
            return static_cast<value_type>(a) > static_cast<value_type>(b) ? static_cast<value_type>(a) : static_cast<value_type>(b);
        }
    }

    template<typename In, typename Min, typename Max>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto clamp(In&& in, Min&& min, Max&& max)
    {
        if constexpr (is_vector_v<In> && is_vector_v<Min> && is_vector_v<Max>)
        {
            using InType = remove_cvr_t<In>;
            using MinType = remove_cvr_t<Min>;
            using MaxType = remove_cvr_t<Max>;
            using scalar = elevated_t<typename InType::scalar, elevated_t<typename MinType::scalar, typename MaxType::scalar>>;
            constexpr size_type inMinSize = InType::size < MinType::size ? InType::size : MinType::size;
            constexpr size_type size = MaxType::size < inMinSize ? MaxType::size : inMinSize;
            return detail::compute_constraint<vector<scalar, size>>::compute_clamp(std::forward<In>(in), std::forward<Min>(min), std::forward<Max>(max));
        }
        else if constexpr (is_vector_v<In> && is_vector_v<Min>)
        {
            using InType = remove_cvr_t<In>;
            using MinType = remove_cvr_t<Min>;
            using scalar = elevated_t<typename InType::scalar, elevated_t<typename MinType::scalar, Max>>;
            constexpr size_type size = InType::size < MinType::size ? InType::size : MinType::size;
            return detail::compute_constraint<vector<scalar, size>>::compute_clamp(std::forward<In>(in), std::forward<Min>(min), std::forward<Max>(max));
        }
        else if constexpr (is_vector_v<In> && is_vector_v<Max>)
        {
            using InType = remove_cvr_t<In>;
            using MaxType = remove_cvr_t<Max>;
            using scalar = elevated_t<typename InType::scalar, elevated_t<Min, typename MaxType::scalar>>;
            constexpr size_type size = InType::size < MaxType::size ? InType::size : MaxType::size;
            return detail::compute_constraint<vector<scalar, size>>::compute_clamp(std::forward<In>(in), std::forward<Min>(min), std::forward<Max>(max));
        }
        else if constexpr (is_vector_v<Min> && is_vector_v<Max>)
        {
            using MinType = remove_cvr_t<Min>;
            using MaxType = remove_cvr_t<Max>;
            using scalar = elevated_t<In, elevated_t<typename MinType::scalar, typename MaxType::scalar>>;
            constexpr size_type size = MinType::size < MaxType::size ? MinType::size : MaxType::size;
            return detail::compute_constraint<vector<scalar, size>>::compute_clamp(std::forward<In>(in), std::forward<Min>(min), std::forward<Max>(max));
        }
        else if constexpr (is_vector_v<In> || is_vector_v<Min> || is_vector_v<Max>)
        {
            return detail::compute_constraint<vector_type_t<In, vector_type_t<Min, Max>>>::compute_clamp(std::forward<In>(in), std::forward<Min>(min), std::forward<Max>(max));
        }
        else
        {
            using value_type = elevated_t<In, elevated_t<Min, Max>>;
            return static_cast<value_type>(in) < static_cast<value_type>(min) ? static_cast<value_type>(min) : (static_cast<value_type>(in) > static_cast<value_type>(max) ? static_cast<value_type>(max) : static_cast<value_type>(in));
        }
    }

    template<typename In>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto saturate(In&& in)
    {
        using value_type = remove_cvr_t<In>;
        if constexpr (is_vector_v<value_type>)
        {
            using scalar = typename value_type::scalar;
            return detail::compute_constraint<make_vector_t<value_type>>::compute_clamp(in, static_cast<scalar>(0), static_cast<scalar>(1));
        }
        else
        {
            return in < static_cast<value_type>(0) ? static_cast<value_type>(0) : (in > static_cast<value_type>(1) ? static_cast<value_type>(1) : static_cast<value_type>(in));
        }
    }
}
