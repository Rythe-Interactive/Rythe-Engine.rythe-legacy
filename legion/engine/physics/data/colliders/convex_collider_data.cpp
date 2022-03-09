#include "convex_collider_data.hpp"

namespace legion::physics
{
    ConvexColliderData::ConvexColliderData(const math::vec3& offset, const math::quat& rotation, const math::vec3& boxExtents,
        const std::bitset<physics_component_flag::pc_max>* changeFlag)
        :
        ColliderData(false,offset,rotation), m_bitsetPtr{ changeFlag },
        m_convexType{ convex_type::box }, m_boxExtents{ boxExtents }
    {

    }
}
