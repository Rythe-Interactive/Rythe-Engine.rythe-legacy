#include <core/particles/particlebuffer.hpp>
#pragma once

namespace legion::core
{
    template<typename bufferType>
    bufferType& particle_buffer<bufferType>::get(size_type idx)
    {
        while (idx >= buffer.size())
            buffer.emplace_back();

        return buffer[idx];
    }

    template<typename bufferType>
    bufferType& particle_buffer<bufferType>::operator[](size_type idx)
    {
        return get(idx);
    }

    template<typename bufferType>
    void particle_buffer<bufferType>::swap(size_type idx1, size_type idx2)
    {
        std::iter_swap(buffer.begin() + idx1, buffer.begin() + idx2);
    }

    template<typename bufferType>
    size_type particle_buffer<bufferType>::size()
    {
        return buffer.size();
    }
}
