#pragma once
#include <core/types/primitives.hpp>
#include <core/math/interpolation/interpolation.hpp>
#include <core/math/basic/basic_funcs.hpp>
#include <core/math/vector/vector.hpp>

namespace legion::core::math::noise
{
    // these primes come from https://github.com/Auburn/FastNoise2
    template<typename integer>
    static constexpr vector<integer, 4> primes = vector<integer, 4>(
        static_cast<integer>(501125321),
        static_cast<integer>(1136930381),
        static_cast<integer>(1720413743),
        static_cast<integer>(1066037191)
    );

    namespace detail
    {
        template<size_type index = 0, size_type resultSize, size_type size, typename int_scalar, typename... ints>
        void xor_impl(vector<int_scalar, resultSize>& hash, const vector<int_scalar, size>& value, const vector<ints, size>&... others)
        {
            for (size_type i = 0; i < size; i++)
                hash[index] ^= value[i];

            xor_impl<index + 1>(hash, others);
        }

        template<typename scalar, size_type size, typename int_scalar = int_least<sizeof(scalar)>, typename... ints>
        static constexpr inline vector<scalar, sizeof...(ints) + 1> hash_normalize(int_scalar seed, const vector<int_scalar, size>& value, const vector<ints, size>&... others)
        {
            vector<int_scalar, sizeof...(ints) + 1> hash(seed);

            detail::xor_impl(hash, value, others...);

            hash *= hash * static_cast<int_scalar>(0x27d4eb2d);
            return static_cast<vector<scalar, sizeof...(ints) + 1>>(hash) * (static_cast<scalar>(1) / static_cast<scalar>(std::numeric_limits<int_scalar>::max()));
        }
    }
}
