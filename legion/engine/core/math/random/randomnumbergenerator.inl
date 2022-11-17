#include <core/math/random/randomnumbergenerator.hpp>
#pragma once

namespace legion::core::math
{
    template<typename T, typename... Args, std::enable_if_t<std::is_base_of_v<random_number_engine_base, T>, bool>>
    inline void RandomNumberGenerator::setRNGEngine(Args&&... args)
    {
        engine = std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename Integer>
    L_NODISCARD inline Integer RandomNumberGenerator::generateRandomInteger() noexcept
    {
        if constexpr (::std::is_signed_v<Integer>)
        {
            using Unsigned = ::std::make_unsigned_t<Integer>;
            static_assert(sizeof(Unsigned) == sizeof(Integer), "Somehow the size of the unsigned version is not the same as the original integer type?");

            Unsigned raw = static_cast<Unsigned>(generate() % static_cast<size_type>(::std::numeric_limits<Unsigned>::max()));
            return *reinterpret_cast<Integer*>(&raw);
        }
        else
            return static_cast<Integer>(generate() % static_cast<size_type>(::std::numeric_limits<Integer>::max()));
    }

    template<typename Float>
    L_NODISCARD inline Float RandomNumberGenerator::generateRandomFloat() noexcept
    {
        auto integer = generateRandomInteger<uint_max>();
        return integer / static_cast<Float>(::std::numeric_limits<uint_max>::max());
    }
}
