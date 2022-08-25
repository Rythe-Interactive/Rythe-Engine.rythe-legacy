#include <core/math/random/random_number_engine.hpp>
#include <chrono>

namespace legion::core::math
{
    thread_local uint64 rythe_default_engine::m_seed = static_cast<uint64>(::std::chrono::high_resolution_clock::now().time_since_epoch().count());

    L_NODISCARD L_ALWAYS_INLINE static constexpr uint64 xor_shift64(uint64 seed) noexcept
    {
        seed ^= seed << 13;
        seed ^= seed >> 7;
        seed ^= seed << 17;
        return seed;
    }

    L_NODISCARD L_ALWAYS_INLINE static constexpr uint64 seed64(size_type seed) noexcept
    {
        if constexpr (sizeof(size_type) != sizeof(uint64))
        {
            constexpr auto convRate = ::std::numeric_limits<uint64>::max() / static_cast<long double>(::std::numeric_limits<size_type>::max());
            return static_cast<uint64>(seed * convRate);
        }
        else
            return static_cast<uint64>(seed);
    }


    void rythe_default_engine::seed(size_type seed) noexcept
    {
        m_seed = seed64(seed);
    };

    void rythe_default_engine::seed(const std::seed_seq& seq) noexcept
    {
        if (seq.size() == 0)
            return;

        std::vector<size_type> seeds(seq.size());
        seq.generate(seeds.begin(), seeds.end());

        m_seed = seed64(seeds[0]);
        for (auto seed : seeds)
        {
            m_seed = xor_shift64(m_seed) ^ seed64(seed);
        }
    };

    L_NODISCARD uint_max rythe_default_engine::generate() noexcept
    {
        return m_seed = xor_shift64(m_seed);
    };

    void rythe_default_engine::discard(size_type n) noexcept
    {
        for (; 0 < n; --n)
            L_MAYBEUNUSED auto temp = generate();
    }
}
