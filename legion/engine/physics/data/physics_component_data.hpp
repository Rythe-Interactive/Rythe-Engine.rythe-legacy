#pragma once
#include <physics/data/colliders/convex_collider_data.hpp>
#include <physics/events/events.hpp>

namespace legion::physics
{
    class physicsComponentData
    {
    public:

        inline void AddBoxCollider(const math::vec3& extents)
        {
            m_convexColliderData.push_back(convexColliderData());
            m_modifyPhysicsComponentEvents.push_back(std::make_unique<add_box_collider>(extents));
        }

    private:

        std::vector<convexColliderData> m_convexColliderData;
        std::vector<std::unique_ptr<core::events::event_base>> m_modifyPhysicsComponentEvents;
    };
}
