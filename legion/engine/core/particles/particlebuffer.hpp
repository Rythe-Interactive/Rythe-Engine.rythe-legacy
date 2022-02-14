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
        std::unordered_map<id_type, void*> buffer;
    public:
        void*& operator[] (size_type& id)
        {
            return buffer[id];
        }

        std::unordered_map<id_type, void*>::iterator begin()
        {
            return buffer.begin();
        }

        std::unordered_map<id_type, void*>::iterator end()
        {
            return buffer.end();
        }

        void emplace(id_type id)
        {
            buffer.emplace(id, nullptr);
        }

        void erase(id_type id)
        {
            buffer.erase(id);
        }
    };

    template<typename bufferType>
    struct particle_buffer : public particle_buffer_base
    {
        particle_buffer() = default;

        bufferType& operator[] (size_type& id)
        {
            return *static_cast<bufferType*>(buffer[id]);
        }

        typename std::unordered_map<id_type, bufferType>::iterator begin()
        {
            return buffer.begin();
        }

        typename std::unordered_map<id_type, bufferType>::iterator end()
        {
            return buffer.end();
        }
    };
}
