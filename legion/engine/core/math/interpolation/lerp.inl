#include <core/math/interpolation/lerp.hpp>
#pragma once

namespace legion::core::math::detail
{
    template<typename V, typename T>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto _lerp_impl_(V&& a, V&& b, T&& t) noexcept
    {
        return a + (b - a) * t;
    }
}

#include <core/math/interpolation/lerp_vector.inl>
//#include <core/math/interpolation/lerp_matrix.inl>
//#include <core/math/interpolation/lerp_quaternion.inl>

namespace legion::core::math
{
    // lerp
    template<typename TypeA, typename TypeB, typename InterpType>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto lerp(TypeA&& a, TypeB&& b, InterpType&& t) noexcept
    {
        using A = remove_cvr_t<TypeA>;
        using B = remove_cvr_t<TypeB>;

        if constexpr (is_quat_v<A> && is_quat_v<B>)
        {
            using scalar = elevated_t<typename A::scalar, typename B::scalar>;
            return detail::compute_lerp<quaternion<scalar>>::compute(::std::forward<TypeA>(a), ::std::forward<TypeB>(b), ::std::forward<InterpType>(t));
        }
        else if constexpr (is_matrix_v<A> && is_matrix_v<B>)
        {
            using scalar = elevated_t<typename A::scalar, typename B::scalar>;
            return detail::compute_lerp<matrix<scalar, A::row_count, A::col_count>>::compute(::std::forward<TypeA>(a), ::std::forward<TypeB>(b), ::std::forward<InterpType>(t));
        }
        else if constexpr (is_vector_v<A> && is_vector_v<B>)
        {
            using scalar = elevated_t<typename A::scalar, typename B::scalar>;
            constexpr size_type size = min(A::size, B::size);
            return detail::compute_lerp<vector<scalar, size>>::compute(::std::forward<TypeA>(a), ::std::forward<TypeB>(b), ::std::forward<InterpType>(t));
        }
        else
        {
            return detail::_lerp_impl_(::std::forward<TypeA>(a), ::std::forward<TypeB>(b), ::std::forward<InterpType>(t));
        }
    }
}
