#pragma once
#include <core/math/basic/constraint.hpp>

namespace legion::core::math::detail
{
    template<typename T>
    struct compute_constraint;

    template<typename Scalar, size_type Size>
    struct compute_constraint<vector<Scalar, Size>>
    {
        using vec_type = vector<Scalar, Size>;

        template<typename AType, typename BType>
        L_NODISCARD L_ALWAYS_INLINE constexpr static auto compute_min(AType&& a, BType&& b)
        {
            using A = remove_cvr_t<AType>;
            using AVec = make_vector_t<AType>;
            using B = remove_cvr_t<BType>;
            using BVec = make_vector_t<BType>;
            vec_type result;
            for (size_type i = 0; i < Size; i++)
                if constexpr (is_vector_v<A> && is_vector_v<B> && AVec::size >= Size && BVec::size >= Size)
                    result[i] = a[i] < b[i] ? a[i] : b[i];
                else if constexpr (is_vector_v<A> && AVec::size >= Size)
                    result[i] = a[i] < static_cast<Scalar>(b) ? a[i] : static_cast<Scalar>(b);
                else
                {
                    static_assert(is_vector_v<B> && BVec::size >= Size);
                    result[i] = static_cast<Scalar>(a) < b[i] ? static_cast<Scalar>(a) : b[i];
                }

            return result;
        }

        template<typename AType, typename BType>
        L_NODISCARD L_ALWAYS_INLINE constexpr static auto compute_max(AType&& a, BType&& b)
        {
            using A = remove_cvr_t<AType>;
            using AVec = make_vector_t<AType>;
            using B = remove_cvr_t<BType>;
            using BVec = make_vector_t<BType>;
            vec_type result;
            for (size_type i = 0; i < Size; i++)
                if constexpr (is_vector_v<A> && is_vector_v<B> && AVec::size >= Size && BVec::size >= Size)
                    result[i] = a[i] > b[i] ? a[i] : b[i];
                else if constexpr (is_vector_v<A> && AVec::size >= Size)
                    result[i] = a[i] > static_cast<Scalar>(b) ? a[i] : static_cast<Scalar>(b);
                else
                {
                    static_assert(is_vector_v<B> && BVec::size >= Size);
                    result[i] = static_cast<Scalar>(a) > b[i] ? static_cast<Scalar>(a) : b[i];
                }

            return result;
        }

        template<typename InType, typename MinType, typename MaxType>
        L_NODISCARD L_ALWAYS_INLINE constexpr static auto compute_clamp(InType&& in, MinType&& min, MaxType&& max)
        {
            using In = remove_cvr_t<InType>;
            using InVec = make_vector_t<InType>;
            using Min = remove_cvr_t<MinType>;
            using MinVec = make_vector_t<MinType>;
            using Max = remove_cvr_t<MaxType>;
            using MaxVec = make_vector_t<MaxType>;
            vec_type result;
            for (size_type i = 0; i < Size; i++)
                if constexpr (is_vector_v<In> && is_vector_v<Min> && is_vector_v<Max> && InVec::size >= Size && MinVec::size >= Size && MaxVec::size >= Size)
                    result[i] = in[i] < min[i] ? min[i] : (in[i] > max[i] ? max[i] : in[i]);
                else if constexpr (is_vector_v<In> && is_vector_v<Min> && InVec::size >= Size && MinVec::size >= Size)
                    result[i] = in[i] < min[i] ? min[i] : (in[i] > static_cast<Scalar>(max) ? static_cast<Scalar>(max) : in[i]);
                else if constexpr (is_vector_v<In> && is_vector_v<Max> && InVec::size >= Size && MaxVec::size >= Size)
                    result[i] = in[i] < static_cast<Scalar>(min) ? static_cast<Scalar>(min) : (in[i] > max[i] ? max[i] : in[i]);
                else if constexpr (is_vector_v<Min> && is_vector_v<Max> && MinVec::size >= Size && MaxVec::size >= Size)
                    result[i] = static_cast<Scalar>(in) < min[i] ? min[i] : (static_cast<Scalar>(in) > max[i] ? max[i] : static_cast<Scalar>(in));
                else if constexpr (is_vector_v<In> && InVec::size >= Size)
                    result[i] = in[i] < static_cast<Scalar>(min) ? static_cast<Scalar>(min) : (in[i] > static_cast<Scalar>(max) ? static_cast<Scalar>(max) : in[i]);
                else if constexpr (is_vector_v<Min> && MinVec::size >= Size)
                    result[i] = static_cast<Scalar>(in) < min[i] ? min[i] : (static_cast<Scalar>(in) > static_cast<Scalar>(max) ? static_cast<Scalar>(max) : static_cast<Scalar>(in));
                else
                {
                    static_assert(is_vector_v<Min> && MinVec::size >= Size);
                    result[i] = static_cast<Scalar>(in) < static_cast<Scalar>(min) ? static_cast<Scalar>(min) : (static_cast<Scalar>(in) > max[i] ? max[i] : static_cast<Scalar>(in));
                }

            return result;
        }
    };
}
