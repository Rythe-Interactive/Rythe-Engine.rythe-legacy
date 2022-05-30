#pragma once
#include <cmath>

#include <core/platform/platform.hpp>
#include <core/types/primitives.hpp>
#include <core/math/util/meta.hpp>

namespace legion::core::math
{
    namespace detail
    {
        template<typename Scalar, size_type Size>
        struct compute_tan
        {
            static constexpr size_type size = Size;
            using vec_type = vector<Scalar, size>;

            L_NODISCARD static vec_type compute(const vec_type& val) noexcept
            {
                vec_type result;
                for (size_type i = 0; i < size; i++)
                    result[i] = static_cast<Scalar>(::std::tan(val));
                return result;
            }
        };

        template<typename Scalar>
        struct compute_tan<Scalar, 1u>
        {
            static constexpr size_type size = 1u;
            using vec_type = vector<Scalar, size>;

            L_NODISCARD static Scalar compute(Scalar val) noexcept
            {
                return ::std::tan(val);
            }
        };
    }

    template<typename T>
    L_NODISCARD L_ALWAYS_INLINE static auto tan(T val)
    {
        using value_type = remove_cvr_t<T>;
        if constexpr (is_vector_v<value_type>)
        {
            return detail::compute_tan<typename value_type::scalar, value_type::size>::compute(val);
        }
        else
        {
            return detail::compute_tan<value_type, 1>::compute(val);
        }
    }
}
