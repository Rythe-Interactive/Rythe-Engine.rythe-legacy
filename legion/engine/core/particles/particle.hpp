#pragma once
#include <core/math/math.hpp>
#include <core/defaults/defaultcomponents.hpp>
/**
 * @file particle.hpp
 * @brief a particle represents all the base attributes of a particle(I.E. id, lifeTime, age, prevPosition, position)
 */

namespace legion::core
{
    struct particle
    {
        id_type id;
        float lifeTime;
        float age;
        position position;
        velocity velocity;
    };

    struct particle_buffer_base
    {
    private:
        std::unordered_map<id_type, void*> buffer;
    public:
        void*& operator[] (id_type& id)
        {
            return buffer[id];
        }
    };

    template<typename bufferType>
    struct particle_buffer : public particle_buffer_base
    {
        particle_buffer()
        {

        }
    };


    struct particle_policy_base
    {

    };
}
