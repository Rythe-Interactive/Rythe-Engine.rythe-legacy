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
        std::unordered_map<id_type, void*> buffer{};

        NO_DTOR_RULE5_NOEXCEPT(particle_buffer_base);
        virtual ~particle_buffer_base() = default;

        void* operator[](id_type id)
        {
            return buffer[id];
        }
    };

    template<typename bufferType>
    struct particle_buffer : public particle_buffer_base
    {
        std::unordered_map<id_type, bufferType> buffer{};

        NO_DTOR_RULE5_NOEXCEPT(particle_buffer);
        ~particle_buffer() = default;

        bufferType& get(id_type id);
    };
}

#include <core/particles/particlebuffer.inl>
