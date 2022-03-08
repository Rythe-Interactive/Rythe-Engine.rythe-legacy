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
        NO_DTOR_RULE5_NOEXCEPT(particle_buffer);
        particle_buffer(size_t size)
        {
            buffer.resize(size);
        }

        ~particle_buffer() = default;

        virtual void swap(size_type idx1, size_type idx2) override;
        virtual size_type size() override;
        virtual void resize(size_type size) override;

        bufferType& get(size_type idx);
        bufferType& operator[](size_type idx);
    };
}

#include <core/particles/particlebuffer.inl>
