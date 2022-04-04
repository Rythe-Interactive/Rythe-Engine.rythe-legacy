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

        void setMass(float mass)
        {
            m_mass = mass;
            m_modificationFlags.set(rigidbody_flag::rb_mass);
        }

        L_ALWAYS_INLINE void setInertiaTensor(const math::mat3& inertiaTensor)
        {
            m_inertiaTensor = inertiaTensor;
            m_modificationFlags.set(rigidbody_flag::rb_inertia_tensor);
        }
        
        L_ALWAYS_INLINE void setVelocity(const math::vec3& velocity)
        {
            m_velocity = velocity;
            m_modificationFlags.set(rigidbody_flag::rb_velocity);
        }

        L_ALWAYS_INLINE void setAngularVelocity(const math::vec3& velocity)
        {
            m_angularVelocity = velocity;
            m_modificationFlags.set(rigidbody_flag::rb_angular_velocity);
        }
        
        L_ALWAYS_INLINE void setLinearDrag(float linearDrag)
        {
            m_linearDrag = linearDrag;
            m_modificationFlags.set(rigidbody_flag::rb_linear_drag);
        }

        L_ALWAYS_INLINE void setAngularDrag(float angularDrag)
        {
            m_angularDrag = angularDrag;
            m_modificationFlags.set(rigidbody_flag::rb_angular_drag);
        }
        
        L_ALWAYS_INLINE float getMass() const noexcept { return m_mass; }

        L_ALWAYS_INLINE math::mat3 getInertiaTensor() const noexcept { return m_inertiaTensor; }

        L_ALWAYS_INLINE math::vec3 getVelocity() const noexcept { return m_velocity; }

        L_ALWAYS_INLINE math::vec3 getAngularVelocity() const noexcept { return m_angularVelocity; }

        L_ALWAYS_INLINE float getLinearDrag() const noexcept { return m_linearDrag; }

        L_ALWAYS_INLINE float getAngularDrag() const noexcept { return m_angularDrag; }

        L_ALWAYS_INLINE float getDensity() const noexcept { return m_density; }

        L_ALWAYS_INLINE const std::bitset<rigidbody_flag::rb_max>& getGeneratedModifyEvents() const
        {
            return m_modificationFlags;
        }

        L_ALWAYS_INLINE void resetModificationFlags() { m_modificationFlags.reset(); }

        //-------------------------------------------- internal use functions ------------------------------------------------//

        /** @brief Internal use only. Sets the mass of the rigidbody without updating its bitset*/
        L_ALWAYS_INLINE void setMassDirect(float newMass) noexcept { m_mass = newMass; }

        /** @brief Internal use only. Sets the density of the rigidbody without updating its bitset.
         * The currently active active physics engine should be responsible for setting this value */
        L_ALWAYS_INLINE void setDensity(float newDensity) noexcept { m_density = newDensity; }

    private:

        math::vec3 m_velocity = math::vec3(0.0);
        math::vec3 m_angularVelocity = math::vec3(0.0);

        math::mat3 m_inertiaTensor = math::mat3(6.0f);

        float m_mass = 1.0f;
        float m_linearDrag;
        float m_angularDrag = 0.01f;

        float m_density = 1.0f;

        std::bitset<rigidbody_flag::rb_max> m_modificationFlags;
    };
}
