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
        virtual size_type size() const noexcept LEGION_PURE;
        virtual void resize(size_type size) LEGION_PURE;
    };

    template<typename bufferType>
    struct particle_buffer : public particle_buffer_base, std::vector<bufferType>
    {
        using std::vector<bufferType>::vector;
        using std::vector<bufferType>::operator=;
        using std::vector<bufferType>::operator[];
        ~particle_buffer() = default;

        virtual void swap(size_type idx1, size_type idx2) override;
        virtual size_type size() const noexcept override;
        virtual void resize(size_type size) override;
    };
}

#include <core/particles/particlebuffer.inl>
