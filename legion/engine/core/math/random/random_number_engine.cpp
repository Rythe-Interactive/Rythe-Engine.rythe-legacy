#include <core/math/random/random_number_engine.hpp>

namespace legion::core::math
{
    thread_local std::mt19937_64 rythe_default_engine::engine;

    void rythe_default_engine::seed(size_type seed) noexcept
    {
        engine.seed(seed);
    };

    void rythe_default_engine::seed(const std::seed_seq& seeds) noexcept
    {
        engine.seed(seeds);
    };

    L_NODISCARD size_type rythe_default_engine::generate() noexcept
    {
        return engine();
    };

    void rythe_default_engine::discard(size_type n) noexcept
    {
        engine.discard(n);
    }
}
