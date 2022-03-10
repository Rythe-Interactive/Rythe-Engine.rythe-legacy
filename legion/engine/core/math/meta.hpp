#pragma once
#include <core/math/vector/vector_base.hpp>
#include <core/math/vector/swizzle/swizzle_base.hpp>
#include <core/math/matrix/matrix_base.hpp>

namespace legion::core::math
{
    template<typename T>
    struct is_vector
    {
        constexpr static bool value = false;
    };

    template<typename Scalar, size_type Size>
    struct is_vector<vector<Scalar, Size>>
    {
        constexpr static bool value = true;
    };

    template<typename Scalar, size_type Size, size_type... args>
    struct is_vector<swizzle<Scalar, Size, args...>>
    {
        constexpr static bool value = true;
    };

    template<typename Scalar>
    struct alignas(sizeof(Scalar) * 4) aligned_vector3;

    template<typename Scalar>
    struct is_vector<aligned_vector3<Scalar>>
    {
        constexpr static bool value = true;
    };

    template<typename T>
    constexpr static bool is_vector_v = is_vector<T>::value;

    template<typename T>
    struct is_matrix
    {
        constexpr static bool value = false;
    };

    template<typename Scalar, size_type RowCount, size_type ColCount>
    struct is_matrix<matrix<Scalar, RowCount, ColCount>>
    {
        constexpr static bool value = true;
    };

    template<typename T>
    constexpr static bool is_matrix_v = is_matrix<T>::value;

    template<typename Scalar>
    struct quaternion;

    template<typename T>
    struct is_quat
    {
        constexpr static bool value = false;
    };

    template<typename Scalar>
    struct is_quat<quaternion<Scalar>>
    {
        constexpr static bool value = true;
    };

    template<typename T>
    constexpr static bool is_quat_v = is_quat<T>::value;

    template<typename FPType>
    struct epsilon
    {
        static constexpr auto value = ::std::numeric_limits<FPType>::epsilon();
    };

    template<typename FPType>
    constexpr auto epsilon_v = epsilon<FPType>::value;

    namespace detail
    {
        template<typename TypeA, typename TypeB>
        constexpr auto highest_epsilon_impl()
        {
            if constexpr (epsilon_v<TypeA> > epsilon_v<TypeB>)
                return epsilon_v<TypeA>;
            else
                return epsilon_v<TypeB>;
        }
    }

    template<typename TypeA, typename TypeB>
    struct highest_epsilon
    {
        constexpr static auto value = detail::highest_epsilon_impl<TypeA, TypeB>();
    };

    template<typename TypeA, typename TypeB>
    constexpr auto highest_epsilon_v = highest_epsilon<TypeA, TypeB>::value;

    template<typename TypeA, typename TypeB>
    struct lowest_precision
    {
        using type = remove_cvr_t<decltype(highest_epsilon_v<TypeA, TypeB>)>;
    };

    template<typename TypeA, typename TypeB>
    using lowest_precision_t = typename lowest_precision<TypeA, TypeB>::type;

    template<typename TypeA, typename TypeB, bool RHS = std::is_floating_point_v<TypeA>>
    struct floating_type;

    template<typename TypeA, typename TypeB>
    struct floating_type<TypeA, TypeB, true>
    {
        using type = TypeA;
    };

    template<typename TypeA, typename TypeB>
    struct floating_type<TypeA, TypeB, false>
    {
        static_assert(std::is_floating_point_v<TypeB>, "Neither types in floating_type was a floating point type.");
        using type = TypeB;
    };

    template<typename TypeA, typename TypeB>
    using floating_type_t = typename floating_type<TypeA, TypeB>::type;
}
