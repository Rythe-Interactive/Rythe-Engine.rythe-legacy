#include "convex_collider_data.hpp"

namespace legion::physics
{
    ColliderData::ColliderData(collider_type colliderType, const math::vec3& offset, const math::quat& rotation,
        const std::bitset<physics_component_flag::pc_max>* modificationBitset)
        :
        m_rotationOffset(rotation), m_positionOffset(offset), m_isRegistered(false),
        m_bitsetPtr{ modificationBitset }, m_colliderType(colliderType)  { }

    ConvexColliderData::ConvexColliderData(const math::vec3& offset, const math::quat& rotation, const math::vec3& boxExtents,
        const std::bitset<physics_component_flag::pc_max>* changeFlag)
        :
        ColliderData(collider_type::box,offset,rotation, changeFlag), m_boxExtents{ boxExtents } { }

    SphereColliderData::SphereColliderData(const math::vec3& offset, float radius,
        const std::bitset<physics_component_flag::pc_max>* modificationBitset)
        :
        ColliderData(collider_type::sphere, offset, math::identity<math::quat>(), modificationBitset), m_radius{radius} { }
}
