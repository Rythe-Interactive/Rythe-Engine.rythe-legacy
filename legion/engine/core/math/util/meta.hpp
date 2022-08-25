#pragma once
#include <type_traits>

#include <core/math/vector/vector_base.hpp>
#include <core/math/vector/swizzle/swizzle_base.hpp>
#include <core/math/matrix/matrix_base.hpp>

namespace legion::core::math
{
    template<typename Scalar>
    struct alignas(sizeof(Scalar) * 4) aligned_vector3;

    struct quaternion_base;

    template<typename Scalar>
    struct quaternion;

    template<>
    struct vector<void, 0>
    {
        using scalar = void;
        static constexpr size_type size = 0;
        using type = vector<void, 0>;
    };

    template<>
    struct quaternion<void>
    {
        using scalar = void;
        static constexpr size_type size = 0;
        using type = quaternion<void>;
        using vec_type = vector<scalar, 3>;
    };

    template<>
    struct matrix<void, 0, 0>
    {
        using scalar = void;
        static constexpr size_type row_count = 0;
        static constexpr size_type col_count = 0;
        static constexpr size_type size = 0;
        using type = matrix<void, 0, 0>;

        using row_type = vector<void, 0>;
    };

    template<typename Scalar, size_type RowCount, size_type ColCount, size_type ColIdx>
    struct column;

    namespace detail
    {
        template<typename T>
        struct _is_vector_impl : ::std::is_base_of<vector_base, T> {};

        template<typename Scalar, size_type Size>
        struct _is_vector_impl<vector<Scalar, Size>> : ::std::true_type {};

        template<typename Scalar, size_type Size, size_type... args>
        struct _is_vector_impl<swizzle<Scalar, Size, args...>> : ::std::true_type {};

        template<typename Scalar>
        struct _is_vector_impl<aligned_vector3<Scalar>> : ::std::true_type {};

        template<typename Scalar, size_type RowCount, size_type ColCount, size_type ColIdx>
        struct _is_vector_impl<column<Scalar, RowCount, ColCount, ColIdx>> : ::std::true_type {};
    }

    template<typename T>
    struct is_vector : detail::_is_vector_impl<remove_cvr_t<T>> {};

    template<typename T>
    constexpr static bool is_vector_v = is_vector<T>::value;

    namespace detail
    {
        template<typename T>
        auto _make_vector_impl()
        {
            if constexpr (is_vector_v<T>)
            {
                return vector<typename T::scalar, T::size>{};
            }
            else if constexpr (::std::is_arithmetic_v<T>)
            {
                return vector<T, 1>{};
            }
            else
            {
                return vector<void, 0>{};
            }
        }
    }

    template<typename T>
    struct make_vector
    {
        using type = remove_cvr_t<decltype(detail::_make_vector_impl<remove_cvr_t<T>>())>;
    };

    template<typename T>
    using make_vector_t = typename make_vector<T>::type;

    namespace detail
    {
        template<typename T>
        struct _is_matrix_impl : ::std::is_base_of<matrix_base, T> {};

        template<typename Scalar, size_type RowCount, size_type ColCount>
        struct _is_matrix_impl<matrix<Scalar, RowCount, ColCount>> : ::std::true_type {};
    }

    template<typename T>
    struct is_matrix : detail::_is_matrix_impl<remove_cvr_t<T>> {};

    template<typename T>
    constexpr static bool is_matrix_v = is_matrix<T>::value;

    namespace detail
    {
        template<typename T>
        auto _make_matrix_impl()
        {
            if constexpr (is_matrix_v<T>)
            {
                return matrix<typename T::scalar, T::row_count, T::col_count>{};
            }
            else if constexpr (::std::is_arithmetic_v<T>)
            {
                return matrix<T, 1, 1>{};
            }
            else
            {
                return matrix<void, 0, 0>{};
            }
        }
    }

    template<typename T>
    struct make_matrix
    {
        using type = remove_cvr_t<decltype(detail::_make_matrix_impl<remove_cvr_t<T>>())>;
    };

    template<typename T>
    using make_matrix_t = typename make_matrix<T>::type;

    namespace detail
    {
        template<typename T>
        struct _is_quat_impl : ::std::is_base_of<quaternion_base, T> {};

        template<typename Scalar>
        struct _is_quat_impl<quaternion<Scalar>> : ::std::true_type {};
    }

    template<typename T>
    struct is_quat : detail::_is_quat_impl<remove_cvr_t<T>> {};

    template<typename T>
    constexpr static bool is_quat_v = is_quat<T>::value;

    namespace detail
    {
        template<typename T>
        auto _make_quat_impl()
        {
            if constexpr (is_quat_v<T>)
            {
                return quaternion<typename T::scalar>{};
            }
            else if constexpr (::std::is_arithmetic_v<T>)
            {
                return quaternion<T>{};
            }
            else
            {
                return quaternion<void>{};
            }
        }
    }

    template<typename T>
    struct make_quat
    {
        using type = remove_cvr_t<decltype(detail::_make_quat_impl<remove_cvr_t<T>>())>;
    };

    template<typename T>
    using make_quat_t = typename make_quat<T>::type;

    template<typename T>
    struct is_linear_algebraic_construct
    {
        constexpr static bool value = is_vector_v<T> || is_matrix_v<T> || is_quat_v<T>;
    };

    template<typename T>
    constexpr bool is_linear_algebraic_construct_v = is_linear_algebraic_construct<T>::value;

    namespace detail
    {
        template<typename T, size_type v>
        struct _uniform_value_impl_;

        template<typename Scalar, size_type Size, size_type v>
        struct _uniform_value_impl_<vector<Scalar, Size>, v>
        {
            constexpr static auto value = vector<Scalar, Size>(static_cast<Scalar>(v));
        };

        template<typename Scalar, size_type Size, size_type... args, size_type v>
        struct _uniform_value_impl_<swizzle<Scalar, Size, args...>, v>
        {
            constexpr static auto value = vector<Scalar, Size>(static_cast<Scalar>(v));
        };

        template<typename Scalar, size_type v>
        struct _uniform_value_impl_<aligned_vector3<Scalar>, v>
        {
            constexpr static auto value = aligned_vector3<Scalar>(static_cast<Scalar>(v));
        };

        template<typename Scalar, size_type RowCount, size_type ColCount, size_type ColIdx, size_type v>
        struct _uniform_value_impl_<column<Scalar, RowCount, ColCount, ColIdx>, v>
        {
            constexpr static auto value = vector<Scalar, RowCount>(static_cast<Scalar>(v));
        };

        template<typename Scalar, size_type v>
        struct _uniform_value_impl_<quaternion<Scalar>, v>
        {
            constexpr static auto value = quaternion<Scalar>(static_cast<Scalar>(v), static_cast<Scalar>(v), static_cast<Scalar>(v), static_cast<Scalar>(v));
        };

        template<typename Scalar, size_type RowCount, size_type ColCount, size_type v>
        struct _uniform_value_impl_<matrix<Scalar, RowCount, ColCount>, v>
        {
            constexpr static auto value = matrix<Scalar, RowCount, ColCount>(static_cast<Scalar>(v), uniform_matrix_signal{});
        };
    }

    template<typename T, size_type v>
    struct uniform_value : detail::_uniform_value_impl_<remove_cvr_t<T>, v> {};

    template<typename T>
    constexpr auto zero = uniform_value<T, 0>::value;

    template<typename T>
    constexpr auto one = uniform_value<T, 1>::value;

    template<typename T, size_type v>
    constexpr auto uniform_value_v = uniform_value<T, v>::value;

    namespace detail
    {
        template<typename FPType>
        struct _epsilon_fp_impl
        {
            static constexpr auto value = ::std::numeric_limits<FPType>::epsilon();
        };

        template<typename IntType>
        struct _epsilon_int_impl : ::std::integral_constant<IntType, 1> {};

        template<typename FPType>
        struct _epsilon_impl : ::std::conditional_t<::std::is_floating_point_v<FPType>, _epsilon_fp_impl<FPType>, _epsilon_int_impl<FPType>> {};
    }

    template<typename FPType>
    struct epsilon : detail::_epsilon_impl<remove_cvr_t<FPType>> {};

    template<typename FPType>
    constexpr auto epsilon_v = epsilon<FPType>::value;

    namespace detail
    {
        template<typename TypeA, typename TypeB>
        constexpr auto _highest_epsilon_impl()
        {
            using A = remove_cvr_t<TypeA>;
            using B = remove_cvr_t<TypeB>;
            if constexpr (epsilon_v<A> > epsilon_v<B>)
                return epsilon_v<A>;
            else
                return epsilon_v<B>;
        }

        template<typename TypeA, typename TypeB>
        constexpr auto _lowest_epsilon_impl()
        {
            using A = remove_cvr_t<TypeA>;
            using B = remove_cvr_t<TypeB>;
            if constexpr (epsilon_v<A> < epsilon_v<B>)
                return epsilon_v<A>;
            else
                return epsilon_v<B>;
        }
    }

    template<typename TypeA, typename TypeB>
    struct highest_epsilon
    {
        constexpr static auto value = detail::_highest_epsilon_impl<TypeA, TypeB>();
    };

    template<typename TypeA, typename TypeB>
    constexpr auto highest_epsilon_v = highest_epsilon<TypeA, TypeB>::value;

    template<typename TypeA, typename TypeB>
    struct lowest_epsilon
    {
        constexpr static auto value = detail::_lowest_epsilon_impl<TypeA, TypeB>();
    };

    template<typename TypeA, typename TypeB>
    constexpr auto lowest_epsilon_v = lowest_epsilon<TypeA, TypeB>::value;

    template<typename TypeA, typename TypeB>
    struct lowest_precision
    {
        using type = remove_cvr_t<decltype(highest_epsilon_v<TypeA, TypeB>)>;
    };

    template<typename TypeA, typename TypeB>
    using lowest_precision_t = typename lowest_precision<TypeA, TypeB>::type;

    template<typename TypeA, typename TypeB>
    struct highest_precision
    {
        using type = remove_cvr_t<decltype(lowest_epsilon_v<TypeA, TypeB>)>;
    };

    template<typename TypeA, typename TypeB>
    using highest_precision_t = typename highest_precision<TypeA, TypeB>::type;

    template<typename TypeA, typename TypeB>
    struct floating_type : ::std::conditional<::std::is_floating_point_v<TypeA> || !::std::is_floating_point_v<TypeB>, remove_cvr_t<TypeA>, remove_cvr_t<TypeB>> {};

    template<typename TypeA, typename TypeB>
    using floating_type_t = typename floating_type<TypeA, TypeB>::type;

    template<typename TypeA, typename TypeB>
    struct vector_type
    {
        using type = make_vector_t<::std::conditional_t<is_vector_v<TypeA> || !is_vector_v<TypeB>, remove_cvr_t<TypeA>, remove_cvr_t<TypeB>>>;
    };

    template<typename TypeA, typename TypeB>
    using vector_type_t = typename vector_type<TypeA, TypeB>::type;

    template<typename T>
    struct make_signed : ::std::make_signed<T> {};

    template<>
    struct make_signed<float>
    {
        using type = float;
    };

    template<>
    struct make_signed<double>
    {
        using type = double;
    };

    template<>
    struct make_signed<long double>
    {
        using type = long double;
    };

    template<typename T>
    using make_signed_t = typename make_signed<T>::type;


    template<typename T>
    struct make_unsigned : ::std::make_unsigned<T> {};

    template<>
    struct make_unsigned<float>
    {
        using type = float;
    };

    template<>
    struct make_unsigned<double>
    {
        using type = double;
    };

    template<>
    struct make_unsigned<long double>
    {
        using type = long double;
    };

    template<typename T>
    using make_unsigned_t = typename make_unsigned<T>::type;

    template<typename A, typename B>
    struct largest : ::std::conditional<(sizeof(A) > sizeof(B)), A, B> {};

    template<typename A, typename B>
    using largest_t = typename largest<A, B>::type;

    template<typename A, typename B>
    struct smallest : ::std::conditional<(sizeof(A) < sizeof(B)), A, B> {};

    template<typename A, typename B>
    using smallest_t = typename smallest<A, B>::type;

    namespace detail
    {
        template<typename A, typename B>
        struct _elevated_int_impl : ::std::conditional<::std::is_signed_v<A> || ::std::is_signed_v<B>, make_signed_t<largest_t<A, B>>, make_unsigned_t<largest_t<A, B>>> {};

        template<typename A, typename B>
        struct _elevated_impl : ::std::conditional<::std::is_floating_point_v<A> || ::std::is_floating_point_v<B>, highest_precision_t<A, B>, typename detail::_elevated_int_impl<A, B>::type> {};
    }

    template<typename A, typename B>
    struct elevated : detail::_elevated_impl<remove_cvr_t<A>, remove_cvr_t<B>> {};

    template<typename A, typename B>
    using elevated_t = typename elevated<A, B>::type;
}
