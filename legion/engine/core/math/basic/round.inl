#include <core/math/basic/round.hpp>
#pragma once

namespace legion::core::math
{
    namespace detail
    {
        enum struct _round_mode_
        {
            floor, round, ceil
        };

        template<typename Target, typename T, _round_mode_ mode>
        L_NODISCARD L_ALWAYS_INLINE constexpr static Target _round_impl_(T val) noexcept
        {
            using value_type = remove_cvr_t<T>;
            if constexpr (::std::is_integral_v<value_type>)
            {
                return static_cast<Target>(val);
            }
            else
            {
                if constexpr (mode == _round_mode_::floor)
                return static_cast<Target>(static_cast<int_max>(val + (sign static_cast<value_type>(0.5))));
            }
        }

        template<typename Scalar, size_type Size>
        struct compute_round
        {
            static constexpr size_type size = Size;
            using value_type = vector<Scalar, size>;

            L_NODISCARD L_ALWAYS_INLINE constexpr static value_type compute(const value_type& val) noexcept
            {
                value_type result;
                for (size_type i = 0; i < size; i++)
                    result[i] = _round_impl_<Scalar>(val[i]);
                return result;
            }

            template<typename Integer>
            L_NODISCARD L_ALWAYS_INLINE constexpr static vector<Integer, size> icompute(const value_type& val) noexcept
            {
                vector<Integer, size> result;
                for (size_type i = 0; i < size; i++)
                    result[i] = _round_impl_<Integer>(val[i]);
                return result;
            }
        };

        template<typename Scalar>
        struct compute_round<Scalar, 1u>
        {
            static constexpr size_type size = 1u;
            using value_type = vector<Scalar, size>;

            L_NODISCARD L_ALWAYS_INLINE constexpr static Scalar compute(Scalar val) noexcept
            {
                return _round_impl_<Scalar>(val);
            }

            template<typename Integer>
            L_NODISCARD L_ALWAYS_INLINE constexpr static Integer icompute(Scalar val) noexcept
            {
                return _round_impl_<Integer>(val);
            }
        };
    }

    template<typename T>
    L_NODISCARD L_ALWAYS_INLINE constexpr static auto round(const T& val) noexcept
    {
        using value_type = remove_cvr_t<T>;
        if constexpr (is_vector_v<value_type>)
        {
            return detail::compute_round<typename value_type::scalar, value_type::size>::compute(val);
        }
        else
        {
            return detail::compute_round<value_type, 1>::compute(val);
        }
    }

    template<typename Integer, typename T>
    L_NODISCARD L_ALWAYS_INLINE constexpr static auto iround(const T& val) noexcept
    {
        using value_type = remove_cvr_t<T>;
        if constexpr (is_vector_v<value_type>)
        {
            return detail::compute_round<typename value_type::scalar, value_type::size>::template icompute<Integer>(val);
        }
        else
        {
            return detail::compute_round<value_type, 1u>::template icompute<Integer>(val);
        }
    }

    template<typename T>
    L_NODISCARD L_ALWAYS_INLINE constexpr static auto uround(const T& val) noexcept
    {
        return iround<uint, T>(val);
    }

    template<typename Scalar>
    L_NODISCARD L_ALWAYS_INLINE constexpr static Scalar ceil(Scalar val) noexcept
    {

    }
}
