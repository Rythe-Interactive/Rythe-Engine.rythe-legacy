#pragma once
#include <core/core.hpp>
#include <physics/data/component_flags.hpp>
#include <bitset>

namespace legion::physics
{
    class CapsuleControllerData
    {
    public:

        void moveTo(const math::vec3& displacement)
        {
            m_modificationFlags.set(capsule_character_flag::cc_move_to, true);
            m_currentDisplacement += displacement;
        }

        void moveToUsingSpeed(const math::vec3& unitDirection)
        {
            m_modificationFlags.set(capsule_character_flag::cc_move_to, true);
            m_currentDisplacement += (unitDirection * m_speed);
        }

        void teleportTo(const math::vec3& position)
        {
            m_modificationFlags.set(capsule_character_flag::cc_teleport_to, true);
            m_currentlTeleportation = position;
        }

        void setGravity(const math::vec3& gravity)
        {
            m_modificationFlags.set(capsule_character_flag::cc_set_gravity, true);
            m_gravity = gravity;
        }

        const math::vec3& getGravity() const noexcept { return m_gravity; }

        void setSpeed(float speed) noexcept { m_speed = speed; }

        float getSpeed() const noexcept { return m_speed; }

        void setRadius(float radius)
        {
            m_modificationFlags.set(capsule_character_flag::cc_set_radius, true);
            m_radius = radius;
        }

        float getRadius() const noexcept { return m_radius; }

        void setHeight(float height)
        {
            m_modificationFlags.set(capsule_character_flag::cc_set_height, true);
            m_height = height;
        }

        float getHeight() const noexcept { return m_height; }

        const math::vec3& getCurrentDisplacement() const noexcept { return m_currentDisplacement; }
        void resetDisplacement() { m_currentDisplacement = math::vec3(0); }

        const std::bitset<capsule_character_flag::cc_max>& getModificationFlags() const noexcept { return m_modificationFlags; }
        void resetModificationFlags() { m_modificationFlags.reset(); }
            
    private:

        math::vec3 m_gravity;
       
        math::vec3 m_currentDisplacement;
        math::vec3 m_currentlTeleportation;

        std::bitset<capsule_character_flag::cc_max> m_modificationFlags;

        float m_radius = 1.0f;
        float m_height = 2.0f;;
        float m_speed = 0.0f;
    };
}
