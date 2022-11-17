#pragma once
#include <core/math/interpolation/lerp.hpp>

namespace legion::core::math::detail
{
    template<typename T>
    struct compute_lerp;

    template<typename Scalar, size_type Size>
    struct compute_lerp<vector<Scalar, Size>>
    {
        using vec_type = vector<Scalar, Size>;

        template<typename AType, typename BType, typename InterpType>
        L_NODISCARD constexpr static auto compute(AType&& a, BType&& b, InterpType&& t) noexcept
        {
            if constexpr (is_vector_v<InterpType>)
                return compute_vector(::std::forward<AType>(a), ::std::forward<BType>(b), ::std::forward<InterpType>(t));
            else
                return compute_scalar(::std::forward<AType>(a), ::std::forward<BType>(b), ::std::forward<InterpType>(t));
        }

    private:
        template<typename AType, typename BType, typename InterpType>
        L_NODISCARD constexpr static auto compute_vector(AType&& a, BType&& b, InterpType&& t) noexcept
        {
            vec_type result;
            size_type j = 0;
            for (size_type i = 0; i < Size; i++)
            {
                if constexpr (is_vector_v<AType> && make_vector_t<AType>::size >= Size)
                {
                    if constexpr (is_vector_v<BType> && make_vector_t<BType>::size >= Size)
                        result[i] = detail::_lerp_impl_(a[i], b[i], t[j]);
                    else
                        result[i] = detail::_lerp_impl_(a[i], b, t[j]);
                }
                else
                {
                    static_assert(is_vector_v<BType> && make_vector_t<BType>::size >= Size);
                    result[i] = detail::_lerp_impl_(a, b[i], t[j]);
                }

                if (j < remove_cvr_t<InterpType>::size - 1)
                    j++;
            }

            return result;
        }

        template<typename AType, typename BType, typename InterpType>
        L_NODISCARD constexpr static auto compute_scalar(AType&& a, BType&& b, InterpType&& t) noexcept
        {
            vec_type result;
            for (size_type i = 0; i < Size; i++)
                if constexpr (is_vector_v<AType> && make_vector_t<AType>::size >= Size)
                {
                    if constexpr (is_vector_v<BType> && make_vector_t<BType>::size >= Size)
                        result[i] = detail::_lerp_impl_(a[i], b[i], ::std::forward<InterpType>(t));
                    else
                        result[i] = detail::_lerp_impl_(a[i], b, ::std::forward<InterpType>(t));
                }
                else
                {
                    static_assert(is_vector_v<BType> && make_vector_t<BType>::size >= Size);
                    result[i] = detail::_lerp_impl_(a, b[i], ::std::forward<InterpType>(t));
                }

            return result;
        }
    };

    template<typename Scalar>
    struct compute_lerp<vector<Scalar, 1u>>
    {
        using vec_type = vector<Scalar, 1u>;

        template<typename AType, typename BType, typename InterpType>
        L_NODISCARD constexpr static Scalar compute(AType&& a, BType&& b, InterpType&& t) noexcept
        {
            if constexpr (is_vector_v<InterpType>)
                return compute_vector(::std::forward<AType>(a), ::std::forward<BType>(b), ::std::forward<InterpType>(t));
            else
                return compute_scalar(::std::forward<AType>(a), ::std::forward<BType>(b), ::std::forward<InterpType>(t));
        }

    private:
        template<typename AType, typename BType, typename InterpType>
        L_NODISCARD constexpr static Scalar compute_vector(AType&& a, BType&& b, InterpType&& t) noexcept
        {
            if constexpr (is_vector_v<AType>)
            {
                if constexpr (is_vector_v<BType>)
                    return detail::_lerp_impl_(a[0], b[0], t[0]);
                else
                    return detail::_lerp_impl_(a[0], b, t[0]);
            }
            else
            {
                static_assert(is_vector_v<BType>);
                return detail::_lerp_impl_(a, b[0], t[0]);
            }
        }

        template<typename AType, typename BType, typename InterpType>
        L_NODISCARD constexpr static Scalar compute_scalar(AType&& a, BType&& b, InterpType&& t) noexcept
        {
            if constexpr (is_vector_v<AType>)
            {
                if constexpr (is_vector_v<BType>)
                    return detail::_lerp_impl_(a[0], b[0], ::std::forward<InterpType>(t));
                else
                    return detail::_lerp_impl_(a[0], b, ::std::forward<InterpType>(t));
            }
            else
            {
                static_assert(is_vector_v<BType>);
                return detail::_lerp_impl_(a, b[0], ::std::forward<InterpType>(t));
            }
        }
    };
}
