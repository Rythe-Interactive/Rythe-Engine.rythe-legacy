#pragma once
#include <core/core.hpp>
#include <core/events/event.hpp>
#include <physics/data/component_flags.hpp>
#include <bitset>

namespace legion::physics
{
    class RigidbodyData
    {
    public:

        inline void setMass(float mass)
        {
            m_mass = mass;
            m_modificationFlags.set(rigidbody_flag::rb_mass);
        }

        inline void setInertiaTensor(const math::mat3& inertiaTensor)
        {
            m_inertiaTensor = inertiaTensor;
            m_modificationFlags.set(rigidbody_flag::rb_inertia_tensor);
        }
        
        inline void setVelocity(const math::vec3& velocity)
        {
            m_velocity = velocity;
            m_modificationFlags.set(rigidbody_flag::rb_velocity);
        }

        inline void setAngularVelocity(const math::vec3& velocity)
        {
            m_angularVelocity = velocity;
            m_modificationFlags.set(rigidbody_flag::rb_angular_velocity);
        }
        
        inline void setLinearDrag(float linearDrag)
        {
            m_linearDrag = linearDrag;
            m_modificationFlags.set(rigidbody_flag::rb_linear_drag);
        }

        inline void setAngularDrag(float angularDrag)
        {
            m_angularDrag = angularDrag;
            m_modificationFlags.set(rigidbody_flag::rb_angular_drag);
        }
        
        inline float getMass() { return m_mass; }

        inline math::mat3 getInertiaTensor() { return m_inertiaTensor; }

        inline math::vec3 getVelocity() { return m_velocity; }

        inline math::vec3 getAngularVelocity() { return m_angularVelocity; }

        inline float getLinearDrag() { return m_linearDrag; }

        inline float getAngularDrag() { return m_angularDrag; }

        inline const std::bitset<rigidbody_flag::rb_max>& getGeneratedModifyEvents() const
        {
            return m_modificationFlags;
        }

        inline void resetModificationFlags() { m_modificationFlags.reset(); }

    private:

        math::vec3 m_velocity = math::vec3(0.0);
        math::vec3 m_angularVelocity = math::vec3(0.0);

        math::mat3 m_inertiaTensor = math::mat3(6.0f);

        float m_mass = 1.0f;
        float m_linearDrag;
        float m_angularDrag = 0.01f;

        std::bitset<rigidbody_flag::rb_max> m_modificationFlags;
    };
}
