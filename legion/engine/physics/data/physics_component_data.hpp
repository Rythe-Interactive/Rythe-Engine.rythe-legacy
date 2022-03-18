#pragma once
#include <physics/data/colliders/collider_data.hpp>
#include <physics/events/events.hpp>
#include <physics/data/component_flags.hpp>
#include <bitset>

namespace legion::physics
{
    constexpr float boxExtentSizeMultiplier = 0.5f;

    class PhysicsComponentData
    {
    public:

        inline static delegate<void* (const std::vector<math::vec3>& vertices)> m_generateConvexColliderFunc =
            [](const std::vector<math::vec3>& vertices) -> void*
        {
            return nullptr;
        };

        L_ALWAYS_INLINE void AddBoxCollider(const math::vec3& extents, const math::vec3& offset, const math::quat& rotation)
        {
            m_convexColliderData.push_back(ConvexColliderData(offset,rotation,extents * boxExtentSizeMultiplier));

            m_colliderCount == 0 ?
                m_modificationFlags.set(physics::physics_component_flag::pc_add_first_box) :
                m_modificationFlags.set(physics::physics_component_flag::pc_add_next_box);

            m_colliderCount++;
        }

        L_ALWAYS_INLINE void AddBoxCollider(const math::vec3& extents)
        {
            AddBoxCollider(extents, math::vec3(0.0f), math::identity<math::quat>());
        }

        L_ALWAYS_INLINE void AddConvexCollider(const std::vector<math::vec3>& vertices, const math::vec3& offset, const math::quat& rotation)
        {
            //convex colliders depend on an external vertex array, needs to be handled immediately 
            void* convexCollider = m_generateConvexColliderFunc(vertices);

            if (convexCollider)
            {
                updateCollider(physics_component_flag::pc_add_first_convex, physics_component_flag::pc_add_next_convex);
            }
            else
            {
                log::error("convex collider generation failed!");
            }
        }
        
        L_ALWAYS_INLINE void AddSphereCollider(float radius, const math::vec3& offset)
        {
           m_sphereColliderData.push_back(SphereColliderData(offset, radius));

           m_colliderCount == 0 ?
               m_modificationFlags.set(physics::physics_component_flag::pc_add_first_sphere) :
               m_modificationFlags.set(physics::physics_component_flag::pc_add_next_sphere);

           m_colliderCount++;
        }

        L_ALWAYS_INLINE const std::bitset<physics_component_flag::pc_max>& getGeneratedModifyEvents() const noexcept
        {
            return m_modificationFlags;
        };

        L_ALWAYS_INLINE std::vector<ConvexColliderData>& getConvexData() noexcept
        {
            return m_convexColliderData;
        }

        L_ALWAYS_INLINE std::vector<SphereColliderData>& getSphereData() noexcept
        {
            return m_sphereColliderData;
        }

        L_ALWAYS_INLINE void resetModificationFlags() { m_modificationFlags.reset(); }

    private:

        inline void updateCollider(physics_component_flag firstColliderFlag, physics_component_flag nextColliderFlag)
        {
            m_colliderCount == 0 ?
                m_modificationFlags.set(firstColliderFlag) :
                m_modificationFlags.set(nextColliderFlag);

            m_colliderCount++;
        }

        std::vector<ConvexColliderData> m_convexColliderData;
        std::vector<SphereColliderData> m_sphereColliderData;

        std::bitset<physics_component_flag::pc_max> m_modificationFlags;

        size_type m_colliderCount = 0;
    };
}
