#pragma once
#include <core/core.hpp>
#include <physics/data/component_flags.hpp>
#include <bitset>

namespace legion::physics
{
    class ColliderData
    {
    public:

        inline void setRegistered(bool registeredState) { m_isRegistered = registeredState; }
        inline bool isRegistered() const { return m_isRegistered; }

        inline const math::vec3& getOffset() const  { return m_positionOffset; }
        inline const math::quat& getRotationOffset() const { return m_rotationOffset; }

        inline collider_type getColliderType() const { return m_colliderType; }

        inline bool isRegisteredOfType(collider_type colliderType) const { return m_isRegistered && colliderType == m_colliderType; }
        
    protected:

        ColliderData(collider_type colliderType, const math::vec3& offset, const math::quat& rotation,
            const std::bitset < physics_component_flag::pc_max>* modificationBitset);
            
        math::quat m_rotationOffset{ math::identity<math::quat>()};
        math::vec3 m_positionOffset{0};

        const std::bitset < physics_component_flag::pc_max>* m_bitsetPtr;

        bool m_isRegistered = false;
        collider_type m_colliderType = collider_type::not_set;
    };

    class ConvexColliderData : public ColliderData
    {
    public:

        ConvexColliderData(const math::vec3& offset,const math::quat& rotation, const math::vec3& boxExtents,
            const std::bitset < physics_component_flag::pc_max>* modificationBitset);

        inline const math::vec3& getBoxExtents() const { return m_boxExtents; }

    private:

        math::vec3 m_boxExtents{0.0f};
    };

    class SphereColliderData : public ColliderData
    {
    public:

        SphereColliderData(const math::vec3& offset, float radius, const std::bitset < physics_component_flag::pc_max>* modificationBitset);

        inline const float getRadius() const { return m_radius; }

    private:

        float m_radius{ 0 };
    };
}

