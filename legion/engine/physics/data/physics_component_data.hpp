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
        typedef delegate<void* (const std::vector<math::vec3>& vertices)> GenerateConvexDelegate;

    public:

        L_ALWAYS_INLINE void addBoxCollider(const math::vec3& extents, const math::vec3& offset, const math::quat& rotation)
        {
            m_convexColliderData.push_back(ConvexColliderData(offset,rotation,extents * boxExtentSizeMultiplier));
            updateColliderRecords(physics_component_flag::pc_add_first_box, physics_component_flag::pc_add_next_box);
        }

        L_ALWAYS_INLINE void addBoxCollider(const math::vec3& extents)
        {
            addBoxCollider(extents, math::vec3(0.0f), math::identity<math::quat>());
        }

        void addConvexCollider(const std::vector<math::vec3>& vertices, const math::vec3& offset, const math::quat& rotation)
        {
            //convex colliders depend on an external vertex array, needs to be handled immediately 
            void* convexColliderPtr = m_generateConvexColliderFunc(vertices);

            if (convexColliderPtr)
            {
                m_convexColliderData.push_back(ConvexColliderData(offset, rotation, convexColliderPtr));
                updateColliderRecords(physics_component_flag::pc_add_first_convex, physics_component_flag::pc_add_next_convex);
            }
            else
            {
                log::error("convex collider generation failed!");
            }
        }
        
        L_ALWAYS_INLINE void addSphereCollider(float radius, const math::vec3& offset)
        {
           m_sphereColliderData.push_back(SphereColliderData(offset, radius));
           updateColliderRecords(physics_component_flag::pc_add_first_sphere, physics_component_flag::pc_add_next_sphere);
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

        static void setConvexGeneratorDelegate(GenerateConvexDelegate generateConvexFunc)
        {
            m_generateConvexColliderFunc = generateConvexFunc;
        }

    private:

        L_ALWAYS_INLINE void updateColliderRecords(physics_component_flag firstColliderFlag, physics_component_flag nextColliderFlag) noexcept
        {
            m_colliderCount == 0 ?
                m_modificationFlags.set(firstColliderFlag) :
                m_modificationFlags.set(nextColliderFlag);

            m_colliderCount++;
        }

        inline static GenerateConvexDelegate m_generateConvexColliderFunc =
            [](const std::vector<math::vec3>& vertices) -> void*
        {
            log::warn("convex collider not generated because PhysicsComponentData::m_generateConvexColliderFunc not set ");
            return nullptr;
        };

        std::vector<ConvexColliderData> m_convexColliderData;
        std::vector<SphereColliderData> m_sphereColliderData;

        std::bitset<physics_component_flag::pc_max> m_modificationFlags;

        size_type m_colliderCount = 0;
    };
}
