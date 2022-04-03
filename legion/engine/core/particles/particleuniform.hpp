#pragma once
#include <any>

#include <core/types/types.hpp>
#include <core/platform/platform.hpp>


namespace legion::core
{
    struct particle_uniform_base
    {
        NO_DTOR_RULE5_NOEXCEPT(particle_uniform_base);
        virtual ~particle_uniform_base() = default;
    };

    template<typename uniformType>
    struct particle_uniform : public particle_uniform_base
    {
    protected:
        uniformType uniform;
    public:
        NO_DTOR_RULE5_NOEXCEPT(particle_uniform);
        particle_uniform(const uniformType& u) : uniform(u) {}
        particle_uniform(uniformType&& u) : uniform(std::move(u)) {}
        ~particle_uniform() = default;

        uniformType& get();
    };
}

#include <core/particles/particleuniform.inl>
