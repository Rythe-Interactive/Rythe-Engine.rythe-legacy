#pragma once
#include <random>

#include <core/platform/platform.hpp>
#include <core/types/primitives.hpp>

namespace legion::core::math
{
    class random_number_engine_base
    {
    public:
        virtual ~random_number_engine_base() = default;
        virtual void seed(size_type seed) noexcept LEGION_PURE;
        virtual void seed(const std::seed_seq& seeds) noexcept LEGION_PURE;
        L_NODISCARD virtual uint_max generate() noexcept LEGION_PURE;
        virtual void discard(size_type n) noexcept LEGION_PURE;
    };

    template<typename Engine>
    class stl_random_engine : public random_number_engine_base
    {
        static thread_local Engine engine;

    public:
        virtual void seed(size_type seed) noexcept override { engine.seed(seed); };
        virtual void seed(const std::seed_seq& seeds) noexcept override { engine.seed(seeds); };
        L_NODISCARD virtual uint_max generate() noexcept override { return engine(); };
        virtual void discard(size_type n) noexcept override { engine.discard(n); }
    };

    template<typename Engine>
    thread_local Engine stl_random_engine<Engine>::engine;

    class rythe_default_engine : public random_number_engine_base
    {
        static thread_local uint64 m_seed;

    public:
        virtual void seed(size_type seed) noexcept override;
        virtual void seed(const std::seed_seq& seeds) noexcept override;
        L_NODISCARD virtual uint_max generate() noexcept override;
        virtual void discard(size_type n) noexcept override;
    };
}
