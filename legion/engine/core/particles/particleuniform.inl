#include <core/particles/particleuniform.hpp>
#pragma once

namespace legion::core
{
    template<typename uniformType>
    uniformType& particle_uniform<uniformType>::get()
    {
        return uniform;
    }
}
