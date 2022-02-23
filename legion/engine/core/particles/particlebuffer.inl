#include <core/particles/particlebuffer.hpp>
#pragma once

namespace legion::core
{
    template<typename bufferType>
    L_ALWAYS_INLINE bufferType& particle_buffer<bufferType>::get(id_type id)
    {
        return buffer[id];
    }
}
