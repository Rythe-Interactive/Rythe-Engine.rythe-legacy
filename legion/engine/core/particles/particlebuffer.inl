#include <core/particles/particlebuffer.hpp>
#pragma once

namespace legion::core
{
    template<typename bufferType>
    bufferType& particle_buffer<bufferType>::at(size_type idx)
    {
        return buffer.at(idx);
    }

    template<typename bufferType>
    bufferType& particle_buffer<bufferType>::operator[](size_type idx)
    {
        return at(idx);
    }

    template<typename bufferType>
    bufferType& particle_buffer<bufferType>::front()
    {
        return at(0);
    }

    template<typename bufferType>
    bufferType& particle_buffer<bufferType>::back()
    {
        return at(end() - 1);
    }

    template<typename bufferType>
    std::vector<bufferType>& particle_buffer<bufferType>::data()
    {
        return buffer;
    }

    template<typename bufferType>
    typename particle_buffer<bufferType>::iterator particle_buffer<bufferType>::begin()
    {
        return buffer.begin();
    }

    template<typename bufferType>
    typename particle_buffer<bufferType>::iterator particle_buffer<bufferType>::end()
    {
        return buffer.end();
    }

    template<typename bufferType>
    size_type particle_buffer<bufferType>::size()
    {
        return buffer.size();
    }

    template<typename bufferType>
    bool particle_buffer<bufferType>::empty()
    {
        return buffer.empty();
    }

    template<typename bufferType>
    void particle_buffer<bufferType>::reserve(size_type amount)
    {
        buffer.reserve(amount);
    }

    template<typename bufferType>
    size_type particle_buffer<bufferType>::capacity()
    {
        return buffer.capacity();
    }

    template<typename bufferType>
    void particle_buffer<bufferType>::swap(size_type idx1, size_type idx2)
    {
        std::swap(buffer[idx1], buffer[idx2]);
    }

    template<typename bufferType>
    void particle_buffer<bufferType>::resize(size_type size)
    {
        buffer.resize(size);
    }

    template<typename bufferType>
    void particle_buffer<bufferType>::clear()
    {
        buffer.clear();
    }

    template<typename bufferType>
    typename particle_buffer<bufferType>::iterator particle_buffer<bufferType>::insert(const particle_buffer<bufferType>::iterator pos, const bufferType& value)
    {
        return buffer.insert(pos, value);
    }

    template<typename bufferType>
    typename particle_buffer<bufferType>::iterator particle_buffer<bufferType>::insert(const particle_buffer<bufferType>::iterator pos, bufferType&& value)
    {
        return buffer.insert(pos, value);
    }

    template<typename bufferType>
    typename particle_buffer<bufferType>::iterator particle_buffer<bufferType>::insert(const particle_buffer<bufferType>::iterator pos, size_type count, const bufferType& value)
    {
        return buffer.insert(pos, count, value);
    }

    template<typename bufferType>
    template<class InputIt>
    void particle_buffer<bufferType>::insert(particle_buffer<bufferType>::iterator pos, InputIt first, InputIt last)
    {
        buffer.insert(pos, first, last);
    }

    template<typename bufferType>
    template<class InputIt>
    typename particle_buffer<bufferType>::iterator particle_buffer<bufferType>::insert(const particle_buffer<bufferType>::iterator pos, InputIt first, InputIt last)
    {
        return buffer.insert(pos, first, last);
    }

    template<typename bufferType>
    typename particle_buffer<bufferType>::iterator particle_buffer<bufferType>::insert(const particle_buffer<bufferType>::iterator pos, std::initializer_list<bufferType> ilist)
    {
        return buffer.insert(pos, ilist);
    }

    template<typename bufferType>
    template<class... Args>
    typename particle_buffer<bufferType>::iterator particle_buffer<bufferType>::emplace(particle_buffer<bufferType>::iterator pos, Args&&... args)
    {
        return buffer.emplace(pos, args);
    }

    template<typename bufferType>
    typename particle_buffer<bufferType>::iterator particle_buffer<bufferType>::erase(particle_buffer<bufferType>::iterator pos)
    {
        return buffer.erase(pos);
    }

    template<typename bufferType>
    typename particle_buffer<bufferType>::iterator particle_buffer<bufferType>::erase(particle_buffer<bufferType>::iterator first, particle_buffer<bufferType>::iterator last)
    {
        return buffer.erase(first, last);
    }

    template<typename bufferType>
    void particle_buffer<bufferType>::push_back(const bufferType& value)
    {
        buffer.push_back(value);
    }

    template<typename bufferType>
    void particle_buffer<bufferType>::push_back(bufferType&& value)
    {
        buffer.push_back(value);
    }

    template<typename bufferType>
    template<class... Args>
    void particle_buffer<bufferType>::emplace_back(Args&&... args)
    {
        buffer.emplace_back(args);
    }

    template<typename bufferType>
    template<class... Args>
    bufferType& particle_buffer<bufferType>::emplace_back(Args&&... args)
    {
        return buffer.emplace_back(args);
    }

    template<typename bufferType>
    void particle_buffer<bufferType>::pop_back()
    {
        buffer.pop_back();
    }
}
