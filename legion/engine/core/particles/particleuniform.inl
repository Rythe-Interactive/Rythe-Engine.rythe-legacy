#include <core/particles/particleuniform.hpp>
#pragma once

namespace legion::core
{
    template<typename uniformType>
    void particle_uniform<uniformType>::set(uniformType val)
    {
        uniform = val;
    }

    template<typename uniformType>
    uniformType particle_uniform<uniformType>::get()
    {
        return std::any_cast<uniformType>(uniform);
    }
}
