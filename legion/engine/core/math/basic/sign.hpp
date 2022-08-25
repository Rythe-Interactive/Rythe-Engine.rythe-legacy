#pragma once
#include <cmath>
#include <limits>

#include <core/platform/platform.hpp>
#include <core/types/primitives.hpp>
#include <core/math/util/meta.hpp>

namespace legion::core::math
{
    template<typename Scalar>
    L_NODISCARD L_ALWAYS_INLINE constexpr Scalar sign(Scalar val) noexcept
    {
        if constexpr (::std::is_signed_v<Scalar>)
            return static_cast<Scalar>(1);
        else
            return static_cast<Scalar>((static_cast<Scalar>(0) <= val) - (val < static_cast<Scalar>(0)));
    }

    template<typename Scalar>
    L_NODISCARD L_ALWAYS_INLINE constexpr Scalar signum(Scalar val) noexcept
    {
        if constexpr (::std::is_signed_v<Scalar>)
            return static_cast<Scalar>(0) < val;
        else
            return static_cast<Scalar>((static_cast<Scalar>(0) < val) - (val < static_cast<Scalar>(0)));
    }

    namespace detail
    {
        template<typename T>
        struct compute_signum;

        template<typename Scalar, size_type Size>
        struct compute_signum<vector<Scalar, Size>>
        {
            static constexpr size_type size = Size;
            using vec_type = vector<Scalar, size>;

            L_NODISCARD L_ALWAYS_INLINE constexpr static vec_type compute(const vec_type& val) noexcept
            {
                vec_type result;
                for (size_type i = 0; i < size; i++)
                    result[i] = signum(val[i]);
                return result;
            }

            L_NODISCARD L_ALWAYS_INLINE constexpr static vec_type compute_sign(const vec_type& val) noexcept
            {
                vec_type result;
                for (size_type i = 0; i < size; i++)
                    result[i] = sign(val[i]);
                return result;
            }
        };

        template<typename Scalar>
        struct compute_signum<vector<Scalar, 1>>
        {
            static constexpr size_type size = 1;
            using vec_type = vector<Scalar, size>;

            L_NODISCARD L_ALWAYS_INLINE constexpr static Scalar compute(const vec_type& val) noexcept
            {
                return signum(val[0]);
            }

            L_NODISCARD L_ALWAYS_INLINE constexpr static Scalar compute_sign(const vec_type& val) noexcept
            {
                return sign(val[0]);
            }
        };
    }

    template<typename vec_type, ::std::enable_if_t<is_vector_v<vec_type>&& vec_type::size != 1, bool> = true>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto sign(const vec_type& val) noexcept
    {
        return detail::compute_signum<make_vector_t<vec_type>>::compute_sign(val);
    }

    template<typename vec_type, ::std::enable_if_t<is_vector_v<vec_type>&& vec_type::size != 1, bool> = true>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto signum(const vec_type& val) noexcept
    {
        return detail::compute_signum<make_vector_t<vec_type>>::compute(val);
    }
}
