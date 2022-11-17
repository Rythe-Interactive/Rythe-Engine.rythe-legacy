#pragma once
#include <core/math/interpolation/map.hpp>

namespace legion::core::math::detail
{
    template<typename T>
    struct compute_quintic;

    template<typename Scalar, size_type Size>
    struct compute_quintic<vector<Scalar, Size>>
    {
        using vec_type = vector<Scalar, Size>;

        template<typename TypeMin, typename TypeMax, typename ValueType>
        L_NODISCARD constexpr static auto compute(TypeMin&& edgeMin, TypeMax&& edgeMax, ValueType&& value) noexcept
        {
            vec_type result;
            for (size_type i = 0; i < Size; i++)
                result[i] = _quintic_impl_(saturate(map01(_extract_item_(value, i), _extract_item_(edgeMin, i), _extract_item_(edgeMax, i))));

            return result;
        }

        template<typename ValueType>
        L_NODISCARD constexpr static auto compute(ValueType&& value) noexcept
        {
            vec_type result;
            for (size_type i = 0; i < Size; i++)
                result[i] = _quintic_impl_(value[i]);

            return result;
        }

        template<typename TypeMin, typename TypeMax, typename ValueType>
        L_NODISCARD constexpr static auto compute_derivative(TypeMin&& edgeMin, TypeMax&& edgeMax, ValueType&& value) noexcept
        {
            vec_type result;
            for (size_type i = 0; i < Size; i++)
                result[i] = _quintic_derivative_impl_(saturate(map01(_extract_item_(value, i), _extract_item_(edgeMin, i), _extract_item_(edgeMax, i))));

            return result;
        }

        template<typename ValueType>
        L_NODISCARD constexpr static auto compute_derivative(ValueType&& value) noexcept
        {
            vec_type result;
            for (size_type i = 0; i < Size; i++)
                result[i] = _quintic_derivative_impl_(value[i]);

            return result;
        }

        template<typename TypeMin, typename TypeMax, typename ValueType>
        L_NODISCARD static auto compute_inverse(TypeMin&& edgeMin, TypeMax&& edgeMax, ValueType&& value) noexcept
        {
            vec_type result;
            for (size_type i = 0; i < Size; i++)
                result[i] = _inverse_quintic_impl_(saturate(map01(_extract_item_(value, i), _extract_item_(edgeMin, i), _extract_item_(edgeMax, i))));

            return result;
        }

        template<typename ValueType>
        L_NODISCARD static auto compute_inverse(ValueType&& value) noexcept
        {
            vec_type result;
            for (size_type i = 0; i < Size; i++)
                result[i] = _inverse_quintic_impl_(value[i]);

            return result;
        }

        template<typename TypeMin, typename TypeMax, typename ValueType>
        L_NODISCARD static auto compute_inverse_derivative(TypeMin&& edgeMin, TypeMax&& edgeMax, ValueType&& value) noexcept
        {
            vec_type result;
            for (size_type i = 0; i < Size; i++)
                result[i] = _inverse_quintic_derivative_impl_(saturate(map01(_extract_item_(value, i), _extract_item_(edgeMin, i), _extract_item_(edgeMax, i))));

            return result;
        }

        template<typename ValueType>
        L_NODISCARD static auto compute_inverse_derivative(ValueType&& value) noexcept
        {
            vec_type result;
            for (size_type i = 0; i < Size; i++)
                result[i] = _inverse_quintic_derivative_impl_(value[i]);

            return result;
        }
    };

    template<typename Scalar>
    struct compute_quintic<vector<Scalar, 1u>>
    {
        using vec_type = vector<Scalar, 1u>;

        template<typename TypeMin, typename TypeMax, typename ValueType>
        L_NODISCARD constexpr static Scalar compute(TypeMin&& edgeMin, TypeMax&& edgeMax, ValueType&& value) noexcept
        {
            return _quintic_impl_(saturate(map01(_extract_item_(value, 0), _extract_item_(edgeMin, 0), _extract_item_(edgeMax, 0))));
        }

        template<typename ValueType>
        L_NODISCARD constexpr static auto compute(ValueType&& value) noexcept
        {
            return _quintic_impl_(value[0]);
        }

        template<typename TypeMin, typename TypeMax, typename ValueType>
        L_NODISCARD constexpr static Scalar compute_derivative(TypeMin&& edgeMin, TypeMax&& edgeMax, ValueType&& value) noexcept
        {
            return _quintic_derivative_impl_(saturate(map01(_extract_item_(value, 0), _extract_item_(edgeMin, 0), _extract_item_(edgeMax, 0))));
        }

        template<typename ValueType>
        L_NODISCARD constexpr static auto compute_derivative(ValueType&& value) noexcept
        {
            return _quintic_derivative_impl_(value[0]);
        }

        template<typename TypeMin, typename TypeMax, typename ValueType>
        L_NODISCARD static Scalar compute_inverse(TypeMin&& edgeMin, TypeMax&& edgeMax, ValueType&& value) noexcept
        {
            return _inverse_quintic_impl_(saturate(map01(_extract_item_(value, 0), _extract_item_(edgeMin, 0), _extract_item_(edgeMax, 0))));
        }

        template<typename ValueType>
        L_NODISCARD static auto compute_inverse(ValueType&& value) noexcept
        {
            return _inverse_quintic_impl_(value[0]);
        }

        template<typename TypeMin, typename TypeMax, typename ValueType>
        L_NODISCARD static Scalar compute_inverse_derivative(TypeMin&& edgeMin, TypeMax&& edgeMax, ValueType&& value) noexcept
        {
            return _inverse_quintic_derivative_impl_(saturate(map01(_extract_item_(value, 0), _extract_item_(edgeMin, 0), _extract_item_(edgeMax, 0))));
        }

        template<typename ValueType>
        L_NODISCARD static auto compute_inverse_derivative(ValueType&& value) noexcept
        {
            return _inverse_quintic_derivative_impl_(value[0]);
        }
    };
}
