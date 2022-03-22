#include "collider_data.hpp"

namespace legion::physics
{
    ColliderData::ColliderData(collider_type colliderType, const math::vec3& offset, const math::quat& rotation) noexcept
        :
        m_rotationOffset(rotation), m_positionOffset(offset), m_isRegistered(false), m_colliderType(colliderType)  { }

    ConvexColliderData::ConvexColliderData(const math::vec3& offset, const math::quat& rotation, const math::vec3& boxExtents) noexcept
        :
        ColliderData(collider_type::box,offset,rotation), m_boxExtents{ boxExtents } { }

    SphereColliderData::SphereColliderData(const math::vec3& offset, float radius) noexcept
        :
        ColliderData(collider_type::sphere, offset, math::identity<math::quat>()), m_radius{radius} { }
        
    ConvexColliderData::ConvexColliderData(const math::vec3& offset, const math::quat& rotation, void* internalConvexColliderStructure)
        :
        ColliderData(collider_type::box, offset, rotation), m_internalConvexStructure{ internalConvexColliderStructure } { }
}
