#pragma once
#include <core/core.hpp>
#include <physics/data/component_flags.hpp>
#include <bitset>

namespace legion::physics
{
    constexpr size_type defaultPhysicsMaterial = 0;

    struct collider_modification_data
    {
        size_type colliderIndex = std::numeric_limits<size_type>::max();
        collider_modification_flag modificationType = collider_modification_flag::cm_max;

        union modification
        {
            math::vec3 newBoxExtents;
            float newRadius;
            size_type newMaterial;
        };

        modification data;
    };

    using InternalConvexColliderPtr = void*;

    class ColliderData
    {
        friend class PhysicsComponentData;

    public:

        ColliderData(size_type colliderIndex,pointer<std::vector<collider_modification_data>> modificationsRequests, collider_type colliderType, const math::vec3& offset, const math::quat& rotation) noexcept;

        L_ALWAYS_INLINE void setRegistered(bool registeredState) noexcept { m_isRegistered = registeredState; }
        L_ALWAYS_INLINE bool isRegistered() const noexcept   { return m_isRegistered; }

        L_ALWAYS_INLINE const math::vec3& getOffset() const noexcept   { return m_positionOffset; }
        L_ALWAYS_INLINE const math::quat& getRotationOffset() const noexcept  { return m_rotationOffset; }

        L_ALWAYS_INLINE collider_type getColliderType() const noexcept  { return m_colliderType; }

        bool isRegisteredOrNotOfType(collider_type colliderType) const noexcept
        {
            return m_isRegistered || colliderType != m_colliderType;
        }

        L_ALWAYS_INLINE size_type getMaterialHash() const noexcept { return m_materialHash; }
        L_ALWAYS_INLINE void setMaterialHash(size_type materialHash) noexcept
        {
            m_materialHash = materialHash;

            collider_modification_data modData;
            modData.colliderIndex = m_colliderIndex;
            modData.modificationType = collider_modification_flag::cm_set_new_material;
            modData.data.newMaterial = materialHash;

            m_modificationsRequests->push_back(modData);
        }

        L_ALWAYS_INLINE pointer<const math::vec3> getBoxExtents() 
        {
            if (m_colliderType != collider_type::box) return { nullptr };

            return { &m_colliderSpecifics.boxExtents };
        }

        void setBoxExtents(const math::vec3& newExtents) noexcept
        {
            collider_modification_data modData;
            modData.colliderIndex = m_colliderIndex;
            modData.modificationType = collider_modification_flag::cm_set_new_box_extents;
            modData.data.newBoxExtents = newExtents;

            m_modificationsRequests->push_back(modData);
        }

        L_ALWAYS_INLINE pointer<const float> getSphereRadius()
        {
            if (m_colliderType != collider_type::sphere) return { nullptr };

            return { &m_colliderSpecifics.sphereRadius };
        }

        void setSphereRadius(float newRadius) noexcept
        {
            collider_modification_data modData;
            modData.colliderIndex = m_colliderIndex;
            modData.modificationType = collider_modification_flag::cm_set_new_sphere_radius;
            modData.data.newRadius = newRadius;

            m_modificationsRequests->push_back(modData);
        }

        L_ALWAYS_INLINE InternalConvexColliderPtr getConvexCollider()
        {
            if (m_colliderType != collider_type::quickhull_convex) return { nullptr };

            return  m_colliderSpecifics.internalConvexStructure;
        }

        L_ALWAYS_INLINE size_type getColliderIndex() const noexcept { return m_colliderIndex; }

    private:

        L_ALWAYS_INLINE void setModificationRequestVector(pointer<std::vector<collider_modification_data>> modificationsRequests) noexcept
        {
            m_modificationsRequests = modificationsRequests;
        }

        L_ALWAYS_INLINE void setColliderToBox(const math::vec3& boxExtents)
        {
            m_colliderSpecifics.boxExtents = { boxExtents };
        }

        L_ALWAYS_INLINE void setColliderToConvexCollider(InternalConvexColliderPtr convexMesh)
        {
            m_colliderSpecifics.internalConvexStructure = { convexMesh };
        }

        L_ALWAYS_INLINE void setColliderToSphereCollider(float radius)
        {
            m_colliderSpecifics.sphereRadius = { radius };
        }

        union ColliderSpecifics
        {
            ColliderSpecifics() : boxExtents{ 0.0f } {}

            math::vec3 boxExtents;
            InternalConvexColliderPtr internalConvexStructure;
            float sphereRadius;
        };

        ColliderSpecifics m_colliderSpecifics;

        math::quat m_rotationOffset{ math::identity<math::quat>()};
        math::vec3 m_positionOffset{0};

        size_type m_materialHash = defaultPhysicsMaterial;
        size_type m_colliderIndex = std::numeric_limits<size_type>::max();

        collider_type m_colliderType = collider_type::not_set;
        pointer<std::vector<collider_modification_data>> m_modificationsRequests{nullptr};

        bool m_isRegistered = false;
    };
}

