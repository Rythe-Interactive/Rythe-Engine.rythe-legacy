#pragma once
#include <core/core.hpp>
#include <physics/data/component_flags.hpp>
#include <bitset>

namespace legion::physics
{
    class ColliderData
    {
    public:
        
        L_ALWAYS_INLINE void setRegistered(bool registeredState) noexcept { m_isRegistered = registeredState; }
        L_ALWAYS_INLINE bool isRegistered() const noexcept   { return m_isRegistered; }

        L_ALWAYS_INLINE const math::vec3& getOffset() const noexcept   { return m_positionOffset; }
        L_ALWAYS_INLINE const math::quat& getRotationOffset() const noexcept  { return m_rotationOffset; }

        L_ALWAYS_INLINE collider_type getColliderType() const noexcept  { return m_colliderType; }

        L_ALWAYS_INLINE bool isRegisteredOfType(collider_type colliderType) const noexcept
        {
            return m_isRegistered && colliderType == m_colliderType;
        }
        
    protected:

        ColliderData(collider_type colliderType, const math::vec3& offset, const math::quat& rotation) noexcept;
            
        math::quat m_rotationOffset{ math::identity<math::quat>()};
        math::vec3 m_positionOffset{0};

        bool m_isRegistered = false;
        collider_type m_colliderType = collider_type::not_set;
    };

    class ConvexColliderData : public ColliderData
    {
    public:
        friend class ConvexColliderHandle;

        ConvexColliderData(const math::vec3& offset,const math::quat& rotation, const math::vec3& boxExtents) noexcept;

        ConvexColliderData(const math::vec3& offset, const math::quat& rotation, void* internalConvexColliderStructure,
            const std::bitset < physics_component_flag::pc_max>* modificationBitset);

        inline const math::vec3& getBoxExtents() const { return m_boxExtents; }
        
        L_ALWAYS_INLINE const math::vec3& getBoxExtents() const noexcept { return m_boxExtents; }

    private:

        math::vec3 m_boxExtents{0.0f};
        void* m_internalConvexStructure{nullptr};
    };

    class SphereColliderData : public ColliderData
    {
    public:

        SphereColliderData(const math::vec3& offset, float radius) noexcept;

        L_ALWAYS_INLINE const float getRadius() const noexcept { return m_radius; }

    private:

        float m_radius{ 0 };
    };
}

