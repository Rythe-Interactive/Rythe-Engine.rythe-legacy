#include <core/particles/particlebuffer.hpp>
#pragma once

namespace legion::core
{
    template<typename bufferType>
    bufferType& particle_buffer<bufferType>::get(size_type idx)
    {
        if (idx < buffer.size())
            return *static_cast<bufferType*>(buffer[idx]);
        else
        {
            while (idx >= buffer.size())
            {
                buffer.emplace_back(new bufferType());
            }
            return *static_cast<bufferType*>(buffer[idx]);
        }
    }

    template<typename bufferType>
    bufferType& particle_buffer<bufferType>::operator[](size_type idx)
    {
        if (idx < buffer.size())
            return *static_cast<bufferType*>(buffer[idx]);
        else
        {
            while (idx >= buffer.size())
            {
                buffer.emplace_back(new bufferType());
            }
            return *static_cast<bufferType*>(buffer[idx]);
        }
    }
}
