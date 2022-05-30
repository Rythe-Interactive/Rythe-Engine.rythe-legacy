#pragma once
#include <core/math/arithmetic/multiplication.hpp>

namespace legion::core::math::detail
{
    template<typename T>
    struct compute_multiplication;

    template<typename Scalar, size_type Size>
    struct compute_multiplication<vector<Scalar, Size>>
    {
        using vec_type = vector<Scalar, Size>;

        template<typename AType, typename BType>
        L_NODISCARD constexpr static auto compute(AType&& a, BType&& b) noexcept
        {
            vec_type result;
            for (size_type i = 0; i < Size; i++)
                if constexpr (is_vector_v<AType> && make_vector_t<AType>::size >= Size)
                {
                    if constexpr (is_vector_v<BType> && make_vector_t<BType>::size >= Size)
                        result[i] = a[i] * b[i];
                    else
                        result[i] = a[i] * b;
                }
                else
                {
                    static_assert(is_vector_v<BType> && make_vector_t<BType>::size >= Size);                
                    result[i] = a * b[i];
                }

            return result;
        }
    };

    template<typename Scalar>
    struct compute_multiplication<vector<Scalar, 1u>>
    {
        using vec_type = vector<Scalar, 1u>;

        template<typename AType, typename BType>
        L_NODISCARD constexpr static Scalar compute(AType&& a, BType&& b) noexcept
        {
            if constexpr (is_vector_v<AType>)
            {
                if constexpr (is_vector_v<BType>)
                    return a[0] * b[0];
                else
                    return a[0] * b;
            }
            else if constexpr (is_vector_v<BType>)
            {
                return a * b[0];
            }
            else
                return a * b;
        }
    };
}
