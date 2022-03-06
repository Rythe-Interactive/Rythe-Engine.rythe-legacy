#pragma once
#include <memory>
#include <type_traits>

#include <core/types/primitives.hpp>

#include <core/math/random/random_number_engine.hpp>

namespace legion::core::math
{
    class RandomNumberGenerator
    {
        static std::unique_ptr<random_number_engine_base> engine;

    public:
        template<typename T, typename... Args, std::enable_if_t<std::is_base_of_v<random_number_engine_base, T>, bool> = true>
        static void setRNGEngine(Args&&... args);

        static void setRNGEngine(std::unique_ptr<random_number_engine_base>&& newEngine);

        static void setSeed(size_type seed);

        static void setSeed(const std::seed_seq& seeds);

        static void discard(size_type n);

        template<typename Integer = int32>
        L_NODISCARD static Integer generateRandomInteger();

        template<typename Float = float32>
        L_NODISCARD static Float generateRandomFloat();

        L_NODISCARD static size_type generate();
    };
}

#include <core/math/random/randomnumbergenerator.inl>
