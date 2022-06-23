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
        using GenerateConvexDelegate = delegate<void* (const std::vector<math::vec3>& vertices)>;

    public:

        PhysicsComponentData() = default;
        ~PhysicsComponentData() = default;

        PhysicsComponentData(const PhysicsComponentData& other) = delete;

        PhysicsComponentData& operator=(const PhysicsComponentData& other) = delete;

        PhysicsComponentData(PhysicsComponentData&& other) noexcept
        {
            m_colliders = std::move(other.m_colliders);
            m_modificationFlags = std::move(other.m_modificationFlags);
            m_colliderCount = other.m_colliderCount;

            for (ColliderData& data : m_colliders)
            {
                data.setModificationRequestVector({ &m_colliderModificationRequests });
            }
        }

        PhysicsComponentData& operator=(PhysicsComponentData&& other)
        {
            m_colliders = std::move(other.m_colliders);
            m_colliderModificationRequests = std::move(other.m_colliderModificationRequests);

            m_modificationFlags = std::move(other.m_modificationFlags);
            
            m_colliderCount = other.m_colliderCount;

            for (size_t i = 0; i < m_colliders.size(); i++)
            {
                m_colliders[i].setModificationRequestVector({&m_colliderModificationRequests});
            }

            return *this;
        }

        L_ALWAYS_INLINE void addBoxCollider(const math::vec3& extents, const math::vec3& offset, const math::quat& rotation)
        {
            if (extents.y == 0.0f)
            {
                DebugBreak();
            }
             
            m_colliders.push_back(ColliderData(m_colliderCount,{ &m_colliderModificationRequests },collider_type::box, offset, rotation));
            m_colliders[m_colliderCount].setColliderToBox(extents * boxExtentSizeMultiplier);

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
                m_colliders.push_back(ColliderData(m_colliderCount, { &m_colliderModificationRequests }, collider_type::quickhull_convex, offset, rotation));
                m_colliders[m_colliderCount].setColliderToConvexCollider(convexColliderPtr);
                updateColliderRecords(physics_component_flag::pc_add_first_convex, physics_component_flag::pc_add_next_convex);
            }
            else
            {
                log::error("convex collider generation failed!");
            }
        }
        
        L_ALWAYS_INLINE void addSphereCollider(float radius, const math::vec3& offset)
        {
           m_colliders.push_back(ColliderData(m_colliderCount, { &m_colliderModificationRequests }, collider_type::sphere, offset, math::identity<math::quat>() ));
           m_colliders[m_colliderCount].setColliderToSphereCollider(radius);
           updateColliderRecords(physics_component_flag::pc_add_first_sphere, physics_component_flag::pc_add_next_sphere);
        }

        L_ALWAYS_INLINE std::vector<ColliderData>& getColliders() noexcept
        {
            return m_colliders;
        }

        L_ALWAYS_INLINE const std::bitset<physics_component_flag::pc_max>& getGeneratedModifyEvents() const noexcept
        {
            return m_modificationFlags;
        };

        L_ALWAYS_INLINE const std::vector<collider_modification_data>& getGeneratedColliderModifyEvents() const noexcept
        {
            return m_colliderModificationRequests;
        };

        L_ALWAYS_INLINE void resetModificationFlags() { m_modificationFlags.reset(); }

        L_ALWAYS_INLINE void resetColliderModificationFlags() { m_colliderModificationRequests.clear(); }
        
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

        std::vector<ColliderData> m_colliders;
        std::vector<collider_modification_data> m_colliderModificationRequests;
        
        inline static GenerateConvexDelegate m_generateConvexColliderFunc =
            [](const std::vector<math::vec3>& vertices) -> void*
        {
            log::warn("convex collider not generated because PhysicsComponentData::m_generateConvexColliderFunc not set ");
            return nullptr;
        };

        std::bitset<physics_component_flag::pc_max> m_modificationFlags;

        size_type m_colliderCount = 0;
    };
}
