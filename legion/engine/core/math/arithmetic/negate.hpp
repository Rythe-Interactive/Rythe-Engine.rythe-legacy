#pragma once
#include <core/types/primitives.hpp>
#include <core/math/vector/vector.hpp>
#include <core/math/util/meta.hpp>

namespace legion::core::math
{
    namespace detail
    {
        template<typename Scalar, size_type Size>
        struct compute_negate
        {
            static constexpr size_type size = Size;
            using value_type = vector<Scalar, size>;

            L_NODISCARD constexpr static value_type compute(const value_type& a) noexcept
            {
                value_type result;
                for (size_type i = 0; i < size; i++)
                    result[i] = -a[i];
                return result;
            }
        };

        template<typename Scalar>
        struct compute_negate<Scalar, 1u>
        {
            static constexpr size_type size = 1u;
            using value_type = vector<Scalar, size>;

            L_NODISCARD constexpr static Scalar compute(Scalar a) noexcept
            {
                return -a;
            }
        };
    }

    template<typename vec_type, std::enable_if_t<is_vector_v<vec_type>, bool> = true>
    L_NODISCARD constexpr auto negate(const vec_type& a) noexcept
    {
        return detail::compute_negate<typename vec_type::scalar, vec_type::size>::compute(a);
    }

    template<typename vec_type, std::enable_if_t<is_vector_v<vec_type>, bool> = true>
    L_NODISCARD constexpr auto operator-(const vec_type& a)
    {
        return detail::compute_negate<typename vec_type::scalar, vec_type::size>::compute(a);
    }

    template<typename vec_type, std::enable_if_t<is_vector_v<vec_type>, bool> = true>
    constexpr vec_type& negate_assign(vec_type& a) noexcept
    {
        return a = detail::compute_negate<typename vec_type::scalar, vec_type::size>::compute(a);
    }
}
