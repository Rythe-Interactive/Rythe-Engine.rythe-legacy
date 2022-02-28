#pragma once
#include <any>

#include <core/types/types.hpp>
#include <core/platform/platform.hpp>


namespace legion::core
{
    struct particle_uniform_base
    {
    protected:
        std::any uniform;
    public:
        NO_DTOR_RULE5_NOEXCEPT(particle_uniform_base);
        virtual ~particle_uniform_base() = default;
    };

    template<typename uniformType>
    struct particle_uniform : public particle_uniform_base
    {
        NO_DTOR_RULE5_NOEXCEPT(particle_uniform);
        ~particle_uniform() = default;

        void set(uniformType val);
        uniformType get();
    };
}

#include <core/particles/particleuniform.inl>