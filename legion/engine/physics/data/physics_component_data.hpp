#pragma once
#include <physics/data/colliders/convex_collider_data.hpp>
#include <physics/events/events.hpp>
#include <physics/data/component_flags.hpp>
#include <bitset>

namespace legion::physics
{
    constexpr float boxExtentSizeMultiplier = 0.5f;

    class physicsComponentData
    {
    public:

        inline void AddBoxCollider(const math::vec3& extents, const math::vec3& offset, const math::quat& rotation)
        {
            m_convexColliderData.push_back(ConvexColliderData(offset,rotation,extents * boxExtentSizeMultiplier,&m_modificationFlags));

            m_colliderCount == 0 ?
                m_modificationFlags.set(physics::physics_component_flag::pc_add_first_box) :
                m_modificationFlags.set(physics::physics_component_flag::pc_add_next_box);

            m_colliderCount++;
        }

        inline void AddBoxCollider(const math::vec3& extents)
        {
            AddBoxCollider(extents, math::vec3(0.0f), math::identity<math::quat>());
        }

        inline void AddSphereCollider(float radius, const math::vec3& offset)
        {
           m_sphereColliderData.push_back(SphereColliderData(offset, radius, &m_modificationFlags));

           m_colliderCount == 0 ?
               m_modificationFlags.set(physics::physics_component_flag::pc_add_first_sphere) :
               m_modificationFlags.set(physics::physics_component_flag::pc_add_next_sphere);

           m_colliderCount++;
        }

        const std::bitset<physics_component_flag::pc_max>& GetGeneratedPhysicsComponentEvents() const
        {
            return m_modificationFlags;
        };

        inline std::vector<ConvexColliderData>& getConvexData()
        {
            return m_convexColliderData;
        }

        inline std::vector<SphereColliderData>& getSphereData()
        {
            return m_sphereColliderData;
        }

        inline void resetModificationFlags() { m_modificationFlags.reset(); }

    private:

        std::vector<ConvexColliderData> m_convexColliderData;
        std::vector<SphereColliderData> m_sphereColliderData;

        std::bitset<physics_component_flag::pc_max> m_modificationFlags;

        size_type m_colliderCount = 0;
    };
}
