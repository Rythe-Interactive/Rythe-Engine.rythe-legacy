#include "physics_helpers.hpp"
#include <physics/events/events.hpp>
namespace legion::physics
{
    delegate<bool(size_type, const char*)> PhysicsHelpers::m_debugMaterialRetrieveCheck =
        [](size_type hash, const char* materialName)->bool
    {
        log::warn("m_debugMaterialRetrieveCheck not set, make sure the currently active physics engine sets this delegate!");
        return true;
    };

    raycastDelegate PhysicsHelpers::m_raycastImpl =
        [](const math::vec3& start, const math::vec3& unitDir, float rayLength, QueryHit& outHit, QueryMask queryMask = QueryMask())
    {
        log::warn("called PhysicsHelpers::raycast but m_raycastImpl not set, make sure the currently active physics engine sets this delegate!");
        return false;
    };

    sweeepBoxDelegate PhysicsHelpers::m_sweepBoxImpl =
        [](const math::vec3& extents, const math::vec3& start, const math::quat& boxOrientation,
            const math::vec3& unitSweepDir, float sweepLength, QueryHit& outHit, QueryMask queryMask)
    {
        log::warn("called PhysicsHelpers::sweepBox but m_sweepBoxImpl not set, make sure the currently active physics engine sets this delegate!");
        return false;
    };

    sweepSphereDelegate PhysicsHelpers::m_sweepSphereImpl =
        [](float radius, const math::vec3& start, const math::vec3& unitDir, float sweepLength, QueryHit& outHit, QueryMask queryMask)
    {
        log::warn("called PhysicsHelpers::sweepSphere but m_sweepSphereImpl not set, make sure the currently active physics engine sets this delegate!");
        return false;
    };

    overlapBoxDelegate PhysicsHelpers::m_overlapBoxImpl =
        [](const math::vec3& extents, const math::vec3& position, const math::quat& orientation, OverlapHit& outOverlap,
            QueryMask queryMask)
    {
        log::warn("called PhysicsHelpers::overlapBox but m_overlapBoxImpl not set, make sure the currently active physics engine sets this delegate!");
        return false;
    };

    overlapSphereDelegate PhysicsHelpers::m_overlapSphereImpl =
        [](float radius, const math::vec3& position, const math::quat& orientation, OverlapHit& outOverlap,
            QueryMask queryMask)
    {
        log::warn("called PhysicsHelpers::overlapSphere but m_overlapSphereImpl not set, make sure the currently active physics engine sets this delegate!");
        return false;
    };

    void PhysicsHelpers::createPhysicsMaterial(float dynamicFriction, float staticFriction, float restitution, const char* name)
    {
        size_type hashResult = nameHash(name);
        events::EventBus::raiseEvent< request_create_physics_material>(request_create_physics_material{ dynamicFriction,staticFriction,restitution,hashResult});
    }

    size_type PhysicsHelpers::retrievePhysicsMaterialHash(const char* name)
    {
        size_type hashResult = nameHash(name);

        #ifdef  _DEBUG
        m_debugMaterialRetrieveCheck.invoke(hashResult, name);
        #endif 

        return hashResult;
    }

    bool PhysicsHelpers::raycast(const math::vec3& start, const math::vec3& unitDir, float rayLength, QueryHit& outHit, QueryMask queryMask)
    {
        return m_raycastImpl(start,unitDir,rayLength,outHit,queryMask);
    }

    bool PhysicsHelpers::sweepBox(const math::vec3& extents, const math::vec3& start, const math::quat& boxOrientation, const math::vec3& unitSweepDir,
        float sweepLength, QueryHit& outHit, QueryMask queryMask)
    {
        return m_sweepBoxImpl(extents, start, boxOrientation, unitSweepDir, sweepLength, outHit, queryMask);
    }

    bool PhysicsHelpers::sweepSphere(float radius, const math::vec3& start, const math::vec3& unitDir, float sweepLength,
        QueryHit& outHit, QueryMask queryMask)
    {
        return m_sweepSphereImpl(radius, start, unitDir, sweepLength, outHit, queryMask);
    }

    bool PhysicsHelpers::checkBoxOverlap(const math::vec3& extents, const math::vec3& position, const math::quat& orientation, OverlapHit& outOverlap, QueryMask queryMask)
    {
        return m_overlapBoxImpl(extents,position,orientation,outOverlap,queryMask);
    }

    bool PhysicsHelpers::checkSphereOverlap(float radius, const math::vec3& position, const math::quat& orientation, OverlapHit& outOverlap, QueryMask queryMask)
    {
        return m_overlapSphereImpl(radius,position,orientation,outOverlap,queryMask);
    }
}
