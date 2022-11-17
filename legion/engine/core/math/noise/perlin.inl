#include <core/math/noise/perlin.hpp>
#pragma once

namespace legion::core::math
{
    namespace detail
    {
        constexpr uint8 _perlin_permutation_[] = { 151,160,137,91,90,15,                    // Hash lookup table as defined by Ken Perlin.  This is a randomly
            131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,    // arranged array of all numbers from 0-255 inclusive.
            190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
            88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
            77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
            102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
            135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
            5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
            223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
            129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
            251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
            49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
            138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
        };

        template<typename scalar>
        L_NODISCARD L_ALWAYS_INLINE constexpr scalar _grad_impl_(uint8 hash, scalar x, scalar y, scalar z) noexcept
        {
            const uint8 h = hash & 15;
            const auto u = h < 8 ? x : y;
            const auto v = h < 4 ? y : (h == 12 || h == 14 ? x : z);
            return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
        }

        template<typename scalar>
        L_NODISCARD L_ALWAYS_INLINE constexpr scalar _perlin_1d_impl_(scalar value, scalar* derivative) noexcept
        {

        }

        template<typename scalar, typename VectorType>
        L_NODISCARD L_ALWAYS_INLINE constexpr scalar _perlin_2d_impl_(VectorType value, vector<scalar, 2>* derivative) noexcept
        {

        }

        template<typename scalar, typename VectorType>
        L_NODISCARD L_ALWAYS_INLINE constexpr scalar _perlin_3d_impl_(VectorType value, vector<scalar, 3>* derivative) noexcept
        {
            const auto floored = ifloor<int32>(value);
            const auto iv = vector<scalar, 3>(floored.x & 255, floored.y & 255, floored.z & 255);
            const auto fv = value - floored;

            const auto coords = smoothstep(fv);
            const auto coordsDerivative = smoothstep_derivative(fv);

            const uint8 A = (_perlin_permutation_[iv.x & 255] + iv.y) & 255;
            const uint8 B = (_perlin_permutation_[(iv.x + 1) & 255] + iv.y) & 255;

            const uint8 AA = (_perlin_permutation_[A] + iv.z) & 255;
            const uint8 AB = (_perlin_permutation_[(A + 1) & 255] + iv.z) & 255;

            const uint8 BA = (_perlin_permutation_[B] + iv.z) & 255;
            const uint8 BB = (_perlin_permutation_[(B + 1) & 255] + iv.z) & 255;

            const auto p0 = _grad_impl_(_perlin_permutation_[AA], fv.x, fv.y, fv.z);
            const auto p1 = _grad_impl_(_perlin_permutation_[BA], fv.x - 1, fv.y, fv.z);
            const auto p2 = _grad_impl_(_perlin_permutation_[AB], fv.x, fv.y - 1, fv.z);
            const auto p3 = _grad_impl_(_perlin_permutation_[BB], fv.x - 1, fv.y - 1, fv.z);
            const auto p4 = _grad_impl_(_perlin_permutation_[(AA + 1) & 255], fv.x, fv.y, fv.z - 1);
            const auto p5 = _grad_impl_(_perlin_permutation_[(BA + 1) & 255], fv.x - 1, fv.y, fv.z - 1);
            const auto p6 = _grad_impl_(_perlin_permutation_[(AB + 1) & 255], fv.x, fv.y - 1, fv.z - 1);
            const auto p7 = _grad_impl_(_perlin_permutation_[(BB + 1) & 255], fv.x - 1, fv.y - 1, fv.z - 1);

            const auto q0 = lerp(p0, p1, u);
            const auto q1 = lerp(p2, p3, u);
            const auto q2 = lerp(p4, p5, u);
            const auto q3 = lerp(p6, p7, u);

            const auto r0 = lerp(q0, q1, v);
            const auto r1 = lerp(q2, q3, v); 

            return perlin_detail::Lerp(r0, r1, w);
        }
    }

    template<typename VectorType>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto perlin(VectorType&& value) noexcept
    {
        constexpr size_type dimensions = make_vector_t<VectorType>::size;
        using scalar = typename make_vector_t<VectorType>::scalar;
        static_assert(is_vector_or_scalar_v<VectorType> && make_vector_t<VectorType>::size <= 3, "Input type must be a vector of size 3 or lower.");
        if constexpr (dimensions == 1)
            return detail::_perlin_1d_impl_(value, nullptr);
        else if constexpr (dimensions == 2)
            return detail::_perlin_2d_impl_(::std::forward<VectorType>(value), nullptr);
        else if constexpr (dimensions == 3)
            return detail::_perlin_3d_impl_(::std::forward<VectorType>(value), nullptr);
        else
            return typename make_vector_t<VectorType>::scalar{};
    }

    template<typename VectorType>
    L_NODISCARD L_ALWAYS_INLINE constexpr auto perlin(VectorType&& value, decay_vector_t<VectorType>& derivative) noexcept
    {
        constexpr size_type dimensions = make_vector_t<VectorType>::size;
        using scalar = typename make_vector_t<VectorType>::scalar;
        static_assert(is_vector_or_scalar_v<VectorType> && make_vector_t<VectorType>::size <= 3, "Input type must be a vector of size 3 or lower.");
        if constexpr (dimensions == 1)
            return detail::_perlin_1d_impl_(value, &derivative);
        else if constexpr (dimensions == 2)
            return detail::_perlin_2d_impl_(::std::forward<VectorType>(value), &derivative);
        else if constexpr (dimensions == 3)
            return detail::_perlin_3d_impl_(::std::forward<VectorType>(value), &derivative);
        else
            return typename make_vector_t<VectorType>::scalar{};
    }
}
