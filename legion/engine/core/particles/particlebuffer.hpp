#pragma once
#include <core/math/math.hpp>
#include <core/defaults/defaultcomponents.hpp>

/**
 * @file particlebuffer.hpp
 * @brief
 */

namespace legion::core
{
    struct particle_buffer_base
    {
    protected:
        std::vector<void*> buffer;
    public:
        NO_DTOR_RULE5_NOEXCEPT(particle_buffer_base);
        virtual ~particle_buffer_base() = default;

        void swap(size_type idx1, size_type idx2);

        size_type size();
    };

    template<typename bufferType>
    struct particle_buffer : public particle_buffer_base
    {
        NO_DTOR_RULE5_NOEXCEPT(particle_buffer);
        particle_buffer(size_t size)
        {
            buffer.resize(size);
        }

        ~particle_buffer() = default;

        bufferType& get(size_type idx);
        bufferType& operator[](size_type idx);
    };
}

#include <core/particles/particlebuffer.inl>
