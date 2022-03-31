#include "collider_data.hpp"

namespace legion::physics
{
    ColliderData::ColliderData(size_type colliderIndex, pointer<std::vector<collider_modification_data>> modificationsRequests, collider_type colliderType,
        const math::vec3& offset, const math::quat& rotation) noexcept
        :
        m_rotationOffset(rotation), m_positionOffset(offset), m_colliderIndex(colliderIndex),
        m_colliderType(colliderType),  m_modificationsRequests(modificationsRequests),m_isRegistered(false) { }

    BoxColliderData::BoxColliderData( const math::vec3& boxExtents) noexcept : m_boxExtents{ boxExtents } { }

    SphereColliderData::SphereColliderData(float radius) noexcept : m_radius{radius} { }
        
    ConvexColliderData::ConvexColliderData(InternalConvexColliderPtr ccPtr) :  m_internalConvexStructure{ ccPtr } { }
}
