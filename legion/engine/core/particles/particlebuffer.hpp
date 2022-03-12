#pragma once
#include <algorithm>

#include <core/platform/platform.hpp>
#include <core/types/types.hpp>

/**
 * @file particlebuffer.hpp
 * @brief
 */

namespace legion::core
{
    struct particle_buffer_base
    {
        NO_DTOR_RULE5_NOEXCEPT(particle_buffer_base);
        virtual ~particle_buffer_base() = default;

        virtual void swap(size_type idx1, size_type idx2) LEGION_PURE;
        virtual size_type size() LEGION_PURE;
        virtual void resize(size_type size) LEGION_PURE;
    };

    template<typename bufferType>
    struct particle_buffer : public particle_buffer_base
    {
    protected:
        std::vector<bufferType> buffer;
    public:
        using iterator = typename std::vector<bufferType>::iterator;

        NO_DTOR_RULE5_NOEXCEPT(particle_buffer);
        particle_buffer(size_t size)
        {
            buffer.resize(size);
        }

        ~particle_buffer() = default;

        //Element access
        bufferType& at(size_type idx);
        bufferType& operator[](size_type idx);
        bufferType& front();
        bufferType& back();
        std::vector<bufferType>& data();

        //Iterators
        iterator begin();
        iterator end();

        //Capacity
        virtual size_type size() override;
        bool empty();
        void reserve(size_type amount);
        size_type capacity();

        //Modifiers
        virtual void swap(size_type idx1, size_type idx2) override;
        virtual void resize(size_type size) override;
        void clear();
        iterator insert(const iterator pos, const bufferType& value);
        iterator insert(const iterator pos, bufferType&& value);
        iterator insert(const iterator pos, size_type count, const bufferType& value);
        template<class InputIt>
        void insert(iterator pos, InputIt first, InputIt last);
        template<class InputIt>
        iterator insert(const iterator pos, InputIt first, InputIt last);
        iterator insert(const iterator pos, std::initializer_list<bufferType> ilist);
        template<class... Args>
        iterator emplace(const iterator pos, Args&&... args);
        iterator erase(iterator pos);
        iterator erase(iterator first, iterator last);
        void push_back(const bufferType& value);
        void push_back(bufferType&& value);
        template<class... Args>
        void emplace_back(Args&&... args);
        template<class... Args>
        bufferType& emplace_back(Args&&... args);
        void pop_back();

    };
}

#include <core/particles/particlebuffer.inl>
