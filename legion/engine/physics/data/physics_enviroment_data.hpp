#pragma once
#include <physics/data/component_flags.hpp>
#include <physics/data/collision_filter.hpp>
#include <bitset>

namespace legion::physics
{
    class PhysicsEnviromentData
    {
    public:

        void instantiateInfinitePlane(const math::vec3& planeNormal, float distFromOrigin)
        {
            if (m_objectType == physics_enviroment_object_type::not_set)
            {
                m_modificationFlags.set(physics_enviroment_flag::pe_add_plane);
                m_planeNormal = planeNormal;
                m_distFromOrigin = distFromOrigin;
            }
            else
            {
                log::warn("Infinite Plane Instantiation Failed! PhysicsEnviromentData is already set");
            }
        }

        L_ALWAYS_INLINE void resetModificationFlags() { m_modificationFlags.reset(); }

        L_ALWAYS_INLINE const std::bitset<physics_enviroment_flag::pe_max>& getGeneratedModifyEvents() const noexcept
        {
            return m_modificationFlags;
        };

        L_ALWAYS_INLINE const math::vec3& getInfinitePlaneNormal() const noexcept { return m_planeNormal; }

        L_ALWAYS_INLINE float getInfinitePlaneDistanceToOrigin() const noexcept { return m_distFromOrigin; }

        L_ALWAYS_INLINE CollisionFilter getCollisionFilter() const noexcept { return m_collisionFilter; }

        L_ALWAYS_INLINE size_type getCollisionMask() const noexcept { return m_mask; }

    private:

        math::vec3 m_planeNormal;
        float m_distFromOrigin;

        std::bitset<physics_enviroment_flag::pe_max> m_modificationFlags;
        CollisionFilter m_collisionFilter;
        size_type m_mask = std::numeric_limits<size_type>::max();

        physics_enviroment_object_type m_objectType = physics_enviroment_object_type::not_set;
    };
}
