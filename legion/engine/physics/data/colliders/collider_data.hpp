#pragma once
#include <core/core.hpp>
#include <physics/data/component_flags.hpp>
#include <bitset>

namespace legion::physics
{
    constexpr size_type defaultPhysicsMaterial = 0;

    struct physics_material
    {
        float dynamicFriction;
        float staticFriction;
        float restitution;
    };

    struct collider_modification_data
    {
        size_type colliderIndex = std::numeric_limits<size_type>::max();
        collider_modification_flag modificationType = collider_modification_flag::cm_max;

        union modification
        {
            math::vec3 newBoxExtents;
            void* newConvexMesh;
            size_type newMaterial;
        };

        modification data;
    };

    class BoxColliderData
    {
    public:

        BoxColliderData() = default;
        BoxColliderData(const math::vec3& boxExtents) noexcept;

        L_ALWAYS_INLINE const math::vec3& getBoxExtents() const noexcept { return m_boxExtents; }

    private:

        math::vec3 m_boxExtents{ 0.0f };
    };

    class ConvexColliderData
    {
    public:

        ConvexColliderData(void* internalConvexColliderStructure);

        L_ALWAYS_INLINE void* getConvexPtr() const noexcept { return m_internalConvexStructure; }

    private:

        void* m_internalConvexStructure{ nullptr };
    };

    class SphereColliderData
    {
    public:

        SphereColliderData(float radius) noexcept;

        L_ALWAYS_INLINE const float getRadius() const noexcept { return m_radius; }

    private:
        float m_radius{ 0 };
    };

    class ColliderData
    {
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

        L_ALWAYS_INLINE void setModificationRequestVector(pointer<std::vector<collider_modification_data>> modificationsRequests) noexcept
        {
            m_modificationsRequests = modificationsRequests;
        }

        L_ALWAYS_INLINE void setColliderToBox(const math::vec3& boxExtents)
        {
            m_colliderSpecifics.boxCollider = { boxExtents };
        }

        L_ALWAYS_INLINE void setColliderToConvexCollider(void* convexMesh)
        {
            m_colliderSpecifics.convexCollider = { convexMesh };
        }

        L_ALWAYS_INLINE void setColliderToSphereCollider(float radius)
        {
            m_colliderSpecifics.sphereCollider = { radius };
        }

        L_ALWAYS_INLINE BoxColliderData& getColliderAsBox() 
        {
            return m_colliderSpecifics.boxCollider;
        }

        L_ALWAYS_INLINE SphereColliderData& getColliderAsSphere()
        {
            return m_colliderSpecifics.sphereCollider;
        }

        L_ALWAYS_INLINE ConvexColliderData& getColliderAsConvex()
        {
            return m_colliderSpecifics.convexCollider;
        }

        L_ALWAYS_INLINE size_type getColliderIndex() const noexcept { return m_colliderIndex; }


    protected:

        union ColliderSpecifics
        {
            ColliderSpecifics() : boxCollider{} {}

            BoxColliderData boxCollider;
            ConvexColliderData convexCollider;
            SphereColliderData sphereCollider;
        };

        ColliderSpecifics m_colliderSpecifics;

        math::quat m_rotationOffset{ math::identity<math::quat>()};
        math::vec3 m_positionOffset{0};

        size_type m_materialHash = defaultPhysicsMaterial;
        size_type m_colliderIndex = std::numeric_limits<size_type>::max();

        collider_type m_colliderType = collider_type::not_set;
        pointer<std::vector<collider_modification_data>> m_modificationsRequests;

        bool m_isRegistered = false;
    };
}

