#pragma once
#include <core/math/util/meta.hpp>
#include <core/math/basic/abs.hpp>

namespace legion::core::math
{
    template <typename TypeA, typename TypeB>
    constexpr bool close_enough(TypeA lhs, TypeB rhs)
    {
        constexpr bool lhsIsInt = std::is_integral_v<TypeA>;
        constexpr bool rhsIsInt = std::is_integral_v<TypeB>;

        if constexpr (lhsIsInt && rhsIsInt)
        {
            return lhs == rhs;
        }
        else if constexpr (lhsIsInt || rhsIsInt)
        {
            using fp_t = floating_type_t<TypeA, TypeB>;
            return abs(static_cast<fp_t>(lhs) - static_cast<fp_t>(rhs)) <= epsilon_v<fp_t>;
        }
        else
        {
            using lowest_t = lowest_precision_t<TypeA, TypeB>;
            return abs(static_cast<lowest_t>(lhs) - static_cast<lowest_t>(rhs)) <= epsilon_v<lowest_t>;
        }
    }
}
