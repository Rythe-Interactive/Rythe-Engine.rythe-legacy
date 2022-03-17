#include <core/particles/particlebuffer.hpp>
#pragma once

namespace legion::core
{
    template<typename bufferType>
    size_type particle_buffer<bufferType>::size() const noexcept
    {
        return std::vector<bufferType>::size();
    }

    template<typename bufferType>
    void particle_buffer<bufferType>::resize(size_type size)
    {
        return std::vector<bufferType>::resize(size);
    }

    template<typename bufferType>
    void particle_buffer<bufferType>::swap(size_type idx1, size_type idx2)
    {
        std::swap(std::vector<bufferType>::at(idx1), std::vector<bufferType>::at(idx2));
    }
}
