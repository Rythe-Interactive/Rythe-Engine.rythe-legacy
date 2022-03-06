#include <core/math/random/randomnumbergenerator.hpp>

namespace legion::core::math
{
    std::unique_ptr<random_number_engine_base> RandomNumberGenerator::engine = ::std::make_unique<rythe_default_engine>();


    void RandomNumberGenerator::setRNGEngine(std::unique_ptr<random_number_engine_base>&& newEngine)
    {
        engine = std::move(newEngine);
    }

    void RandomNumberGenerator::setSeed(size_type seed)
    {
        engine->seed(seed);
    }

    void RandomNumberGenerator::setSeed(const std::seed_seq& seeds)
    {
        engine->seed(seeds);
    }

    void RandomNumberGenerator::discard(size_type n)
    {
        engine->discard(n);
    }

    L_NODISCARD size_type RandomNumberGenerator::generate()
    {
        return engine->generate();
    }
}
