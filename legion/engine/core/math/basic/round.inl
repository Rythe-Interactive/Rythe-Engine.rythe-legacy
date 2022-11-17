#include <core/math/basic/round.hpp>
#pragma once

namespace legion::core::math
{
    namespace detail
    {
        template<typename Target, round_mode Mode, typename T>
        L_NODISCARD L_ALWAYS_INLINE constexpr Target _round_impl_(T val) noexcept
        {
            using value_type = remove_cvr_t<T>;
            if constexpr (::std::is_integral_v<value_type>)
            {
                return static_cast<Target>(val);
            }
            else
            {
                value_type pretrunc = static_cast<value_type>(0);
                if constexpr (Mode == round_mode::floor)
                    pretrunc = val - static_cast<value_type>(val < static_cast<value_type>(0));
                else if constexpr (Mode == round_mode::round)
                    pretrunc = val + (sign(val) * static_cast<value_type>(0.5));
                else if constexpr (Mode == round_mode::ceil)
                    pretrunc = val + (sign(val) * static_cast<value_type>(1.0 - epsilon_v<value_type>));
                else if constexpr (Mode == round_mode::trunc)
                    pretrunc = val;

                if constexpr (::std::is_signed_v<Target>)
                    return static_cast<Target>(static_cast<int_max>(pretrunc));
                else
                    return pretrunc < static_cast<value_type>(0) ? static_cast<Target>(0) : static_cast<Target>(static_cast<uint_max>(pretrunc));
            }
        }

        template<typename T, round_mode Mode>
        struct compute_round;

        template<typename Scalar, size_type Size, round_mode Mode>
        struct compute_round<vector<Scalar, Size>, Mode>
        {
            static constexpr size_type size = Size;
            using value_type = vector<Scalar, size>;

            template<typename Target>
            L_NODISCARD L_ALWAYS_INLINE constexpr static auto compute(const value_type& val) noexcept
            {
                vector<Target, size> result;
                for (size_type i = 0; i < size; i++)
                    result[i] = _round_impl_<Target, Mode>(val[i]);
                return result;
            }
        };

        template<typename Scalar, round_mode Mode>
        struct compute_round<vector<Scalar, 1>, Mode>
        {
            static constexpr size_type size = 1;
            using value_type = vector<Scalar, size>;

            template<typename Target>
            L_NODISCARD L_ALWAYS_INLINE constexpr static auto compute(const value_type& val) noexcept
            {
                return _round_impl_<Target, Mode>(val[0]);
            }
        };

        template<round_mode Mode, typename T>
        L_NODISCARD L_ALWAYS_INLINE constexpr auto _auto_adv_round_(T&& val) noexcept
        {
            using value_type = remove_cvr_t<T>;
            if constexpr (is_vector_v<value_type>)
            {
                return detail::compute_round<make_vector_t<value_type>, Mode>::template compute<typename value_type::scalar>(val);
            }
            else
            {
                return detail::_round_impl_<value_type, Mode>(val);
            }
        }
    }

    template<typename Target, round_mode Mode, typename T>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto adv_round(T&& val) noexcept
    {
        using value_type = remove_cvr_t<T>;
        if constexpr (is_vector_v<value_type>)
        {
            return detail::compute_round<make_vector_t<value_type>, Mode>::template compute<Target>(val);
        }
        else
        {
            return detail::_round_impl_<Target, Mode>(val);
        }
    }

    template<typename T>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto round(T&& val) noexcept
    {
        return detail::_auto_adv_round_<round_mode::round>(val);
    }

    template<typename Integer, typename T>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto iround(T&& val) noexcept
    {
        return adv_round<Integer, round_mode::round>(val);
    }

    template<typename T>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto uround(T&& val) noexcept
    {
        return adv_round<uint, round_mode::round>(val);
    }

    template<typename T>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto ceil(T&& val) noexcept
    {
        return detail::_auto_adv_round_<round_mode::ceil>(val);
    }

    template<typename Integer, typename T>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto iceil(T&& val) noexcept
    {
        return adv_round<Integer, round_mode::ceil>(val);
    }

    template<typename T>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto uceil(T&& val) noexcept
    {
        return adv_round<uint, round_mode::ceil>(val);
    }

    template<typename T>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto floor(T&& val) noexcept
    {
        return detail::_auto_adv_round_<round_mode::floor>(val);
    }

    template<typename Integer, typename T>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto ifloor(T&& val) noexcept
    {
        return adv_round<Integer, round_mode::floor>(val);
    }

    template<typename T>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto ufloor(T&& val) noexcept
    {
        return adv_round<uint, round_mode::floor>(val);
    }

    template<typename T>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto trunc(T&& val) noexcept
    {
        return detail::_auto_adv_round_<round_mode::trunc>(val);
    }

    template<typename Integer, typename T>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto itrunc(T&& val) noexcept
    {
        return adv_round<Integer, round_mode::trunc>(val);
    }

    template<typename T>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto utrunc(T&& val) noexcept
    {
        return adv_round<uint, round_mode::trunc>(val);
    }
}
