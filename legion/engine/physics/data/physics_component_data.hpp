#pragma once
#include <physics/data/colliders/convex_collider_data.hpp>
#include <physics/events/events.hpp>

namespace legion::physics
{
    constexpr float boxExtentSizeMultiplier = 0.5f;

    class physicsComponentData
    {
    public:

        inline void AddBoxCollider(const math::vec3& extents)
        {
            m_convexColliderData.push_back(convexColliderData());
            m_modifyPhysicsComponentEvents.push_back(std::make_unique<add_box_collider>(extents * boxExtentSizeMultiplier));
        }

        std::vector<std::unique_ptr<core::events::event_base>>& GetGeneratedPhysicsComponentEvents()
        {
            return m_modifyPhysicsComponentEvents;
        };

    private:

        std::vector<convexColliderData> m_convexColliderData;
        std::vector<std::unique_ptr<core::events::event_base>> m_modifyPhysicsComponentEvents;
    };
}
