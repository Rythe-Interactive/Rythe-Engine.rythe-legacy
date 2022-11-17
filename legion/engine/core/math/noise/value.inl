#include <core/math/noise/value.hpp>
#pragma once

namespace legion::core::math::noise
{
    namespace detail
    {
        template<typename scalar, typename int_scalar = int_least<sizeof(scalar)>>
        L_NODISCARD L_ALWAYS_INLINE constexpr scalar _value_1d_impl_(int_scalar seed, scalar value) noexcept
        {
            int_scalar floored = ifloor<int_scalar>(value);
            int_scalar start = floored * primes<int_scalar>.x;

            return lerp(detail::hash_normalize<scalar>(seed, start), detail::hash_normalize<scalar>(seed, start + primes<int_scalar>::x), smoothstep(value - static_cast<scalar>(floored)));
        }

        template<typename scalar, typename int_scalar = int_least<sizeof(scalar)>>
        L_NODISCARD L_ALWAYS_INLINE constexpr scalar _value_1d_impl_(int_scalar seed, scalar value, scalar& derivative) noexcept
        {
            int_scalar floored = ifloor<int_scalar>(value);
            int_scalar start = floored * primes<int_scalar>.x;

            scalar _t = value - static_cast<scalar>(floored);
            scalar t = smoothstep(_t);
            scalar dt = smoothstep_derivative(_t);

            scalar gradStart = detail::hash_normalize<scalar>(seed, start);
            scalar gradEnd = detail::hash_normalize<scalar>(seed, start + primes<int_scalar>::x);

            derivative = (gradEnd - gradStart) * dt;

            return lerp(gradStart, gradEnd, t);
        }

        template<typename scalar, typename int_scalar = int_least<sizeof(scalar)>, typename VectorType>
        L_NODISCARD L_ALWAYS_INLINE constexpr scalar _value_2d_impl_(int_scalar seed, VectorType value) noexcept
        {
            using scalar2 = vector<scalar, 2>;
            using int_scalar2 = vector<int_scalar, 2>;

            int_scalar2 floored = ifloor<int_scalar>(value);
            int_scalar2 start = floored * primes<int_scalar>.xy;
            int_scalar2 end = start + primes<int_scalar>.xy;

            scalar2 t = smoothstep(value - static_cast<scalar2>(floored));

            scalar2 horizontal = lerp(detail::hash_normalize<scalar>(seed, start,                       int_scalar2(start.x, end.y)),
                                      detail::hash_normalize<scalar>(seed, int_scalar2(end.x, start.y), end), t.x);

            return lerp(horizontal.x, horizontal.y, t.y);
        }

        template<typename scalar, typename int_scalar = int_least<sizeof(scalar)>, typename VectorType>
        L_NODISCARD L_ALWAYS_INLINE constexpr scalar _value_2d_impl_(int_scalar seed, VectorType value, vector<scalar, 2>& derivative) noexcept
        {
            using scalar2 = vector<scalar, 2>;
            using int_scalar2 = vector<int_scalar, 2>;

            int_scalar2 floored = ifloor<int_scalar>(value);
            int_scalar2 start = floored * primes<int_scalar>.xy;
            int_scalar2 end = start + primes<int_scalar>.xy;

            scalar2 _t = value - static_cast<scalar2>(floored);
            scalar2 t = smoothstep(_t);
            scalar2 dt = smoothstep_derivative(_t);

            scalar2 ac = detail::hash_normalize<scalar>(seed, start, int_scalar2(end.x, start.y));
            scalar2 bd = detail::hash_normalize<scalar>(seed, int_scalar2(start.x, end.y), end);
            scalar2 vertical = lerp(ac, bd, t.y);

            derivative.x = (vertical.y - vertical.x) * dt.x;
            scalar2 dyT = (bd - ac) * dt.y;
            derivative.y = lerp(dyT.x, dyT.y, t.x);

            return lerp(vertical.x, vertical.y, t.x);

        }

        template<typename scalar, typename int_scalar = int_least<sizeof(scalar)>, typename VectorType>
        L_NODISCARD L_ALWAYS_INLINE constexpr scalar _value_3d_impl_(int_scalar seed, VectorType value) noexcept
        {
            using scalar3 = vector<scalar, 3>;
            using int_scalar3 = vector<int_scalar, 3>;

            int_scalar3 floored = ifloor<int_scalar>(value);
            int_scalar3 start = floored * primes<int_scalar>.xyz;
            int_scalar3 end = start + primes<int_scalar>.xyz;

            scalar3 t = smoothstep(value - static_cast<scalar3>(floored));

            vector<scalar, 4> i = lerp(detail::hash_normalize<scalar>(seed, start,                                int_scalar3(start.x, end.y, start.z), int_scalar3(start.x, start.y, end.z), int_scalar3(start.x, end.y, end.z)),
                                       detail::hash_normalize<scalar>(seed, int_scalar3(end.x, start.y, start.z), int_scalar3(end.x, end.y, start.z),   int_scalar3(end.x, start.y, end.z),   end), t.x);

            vector<scalar, 2> j = lerp(i.xz, i.yw, t.y);

            return lerp(j.x, j.y, t.z);
        }

        template<typename scalar, typename int_scalar = int_least<sizeof(scalar)>, typename VectorType>
        L_NODISCARD L_ALWAYS_INLINE constexpr scalar _value_3d_impl_(int_scalar seed, VectorType value, vector<scalar, 3>& derivative) noexcept
        {
            using scalar2 = vector<scalar, 2>;
            using scalar3 = vector<scalar, 3>;
            using scalar4 = vector<scalar, 4>;
            using int_scalar3 = vector<int_scalar, 3>;

            int_scalar3 floored = ifloor<int_scalar>(value);
            int_scalar3 start = floored * primes<int_scalar>.xyz;
            int_scalar3 end = start + primes<int_scalar>.xyz;

            scalar3 _t = value - static_cast<scalar3>(floored);
            scalar3 t = smoothstep(_t);
            scalar3 dt = smoothstep_derivative(_t);

            scalar4 aceg = detail::hash_normalize<scalar>(seed, start,                                int_scalar3(start.x, end.y, start.z), int_scalar3(end.x, start.y, start.z), int_scalar3(end.x, end.y, start.z));
            scalar4 bdfh = detail::hash_normalize<scalar>(seed, int_scalar3(start.x, start.y, end.z), int_scalar3(start.x, end.y, end.z),   int_scalar3(end.x, start.y, end.z),   end);

            scalar4 k = lerp(aceg, bdfh, t.z);

            scalar2 j = lerp(k.xz, k.yw, t.y);

            derivative.x = (j.y - j.x) * dt.x;
            scalar2 dyT = (k.yw - k.xz) * dt.y;
            derivative.y = lerp(dyT.x, dyT.y, t.x);
            scalar4 dzT = (bdfh - aceg) * dt.zero;
            scalar2 dzTT = lerp(dzT.xz, dzT.yw, t.y);
            derivative.z = lerp(dzTT.x, dzTT.y, t.x);

            return lerp(j.x, j.y, t.x);
        }
    }

    template<typename VectorType>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto value(VectorType&& value, int_least<sizeof(vector_scalar_t<VectorType>)> seed = 0) noexcept
    {
        constexpr size_type dimensions = vector_size_v<VectorType>;
        using scalar = vector_scalar_t<VectorType>;
        static_assert(is_vector_or_scalar_v<VectorType> && make_vector_t<VectorType>::size <= 3, "Input type must be a vector of size 3 or lower.");
        if constexpr (dimensions == 1)
            return detail::_value_1d_impl_(seed, value);
        else if constexpr (dimensions == 2)
            return detail::_value_2d_impl_(seed, ::std::forward<VectorType>(value));
        else if constexpr (dimensions == 3)
            return detail::_value_3d_impl_(seed, ::std::forward<VectorType>(value));
        else
            return typename make_vector_t<VectorType>::scalar{};
    }

    template<typename VectorType>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto value(VectorType&& value, decay_vector_t<VectorType>& derivative, int_least<sizeof(vector_scalar_t<VectorType>)> seed = 0) noexcept
    {
        constexpr size_type dimensions = vector_size_v<VectorType>;
        using scalar = vector_scalar_t<VectorType>;
        static_assert(is_vector_or_scalar_v<VectorType> && dimensions <= 3, "Input type must be a vector of size 3 or lower.");
        if constexpr (dimensions == 1)
            return detail::_value_1d_impl_(seed, value, derivative);
        else if constexpr (dimensions == 2)
            return detail::_value_2d_impl_(seed, ::std::forward<VectorType>(value), derivative);
        else if constexpr (dimensions == 3)
            return detail::_value_3d_impl_(seed, ::std::forward<VectorType>(value), derivative);
        else
            return typename make_vector_t<VectorType>::scalar{};
    }
}
