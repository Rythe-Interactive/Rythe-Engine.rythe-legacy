#pragma once
#include <core/core.hpp>
#include <physics/data/query_mask.hpp>
#include <physics/data/query_result.hpp>
#include <physics/data/colliders/collider_data.hpp>

namespace legion::physics
{
    typedef delegate<bool(const math::vec3&, const math::vec3&, float, QueryHit&, QueryMask)> raycastDelegate;

    typedef delegate<bool(const math::vec3&, const math::vec3&, const math::quat&,
        const math::vec3&, float, QueryHit&, QueryMask)> sweeepBoxDelegate;

    typedef delegate<bool(float, const math::vec3&, const math::vec3&, float, QueryHit&, QueryMask)> sweepSphereDelegate;

    typedef delegate<bool(const math::vec3&, const math::vec3&, const math::quat&, OverlapHit&, QueryMask)> overlapBoxDelegate;

    typedef delegate<bool(float, const math::vec3&, const math::quat&, OverlapHit&, QueryMask)> overlapSphereDelegate;

    class PhysicsHelpers
    {
    public:

        static void createPhysicsMaterial(float dynamicFriction, float staticFriction, float restitution, const char* name);

        static size_type retrievePhysicsMaterialHash(const char* name);

        static void bindDebugRetrieveCheck(delegate<bool(size_type, const char*)> retrieveDelegate)
        {
            m_debugMaterialRetrieveCheck = retrieveDelegate;
        }

        static void bindRaycastFunction(raycastDelegate raycastDelegate) { m_raycastImpl = raycastDelegate; }

        static void bindBoxSweepFunction(sweeepBoxDelegate sweepBoxDelegate) { m_sweepBoxImpl = sweepBoxDelegate; }

        static void bindSweepSphereFunction(sweepSphereDelegate sweepSphereDelegate) { m_sweepSphereImpl = sweepSphereDelegate; }

        static void bindOverlapBoxFunction(overlapBoxDelegate overlapBoxDelegate) { m_overlapBoxImpl = overlapBoxDelegate; }

        static void bindOverlapSphereFunction(overlapSphereDelegate overlapSphereDelegate) { m_overlapSphereImpl = overlapSphereDelegate; }

        static bool raycast(const math::vec3& start, const math::vec3& unitDir, float rayLength, QueryHit& outHit, QueryMask queryMask = QueryMask());

        static bool sweepBox(const math::vec3& extents,const math::vec3& start, const math::quat& boxOrientation,
            const math::vec3& unitSweepDir, float sweepLength, QueryHit& outHit, QueryMask queryMask = QueryMask());

        static bool sweepSphere(float radius, const math::vec3& start, const math::vec3& unitDir, float sweepLength, QueryHit& outHit,
            QueryMask queryMask = QueryMask());

        static bool checkBoxOverlap(const math::vec3& extents, const math::vec3& position, const math::quat& orientation, OverlapHit& outOverlap,
            QueryMask queryMask = QueryMask());

        static bool checkSphereOverlap(float radius, const math::vec3& position, const math::quat& orientation, OverlapHit& outOverlap,
            QueryMask queryMask = QueryMask());

    private:

        PhysicsHelpers() = default;

        static delegate<bool(size_type,const char*)> m_debugMaterialRetrieveCheck;

        static sweeepBoxDelegate m_sweepBoxImpl;
        static sweepSphereDelegate m_sweepSphereImpl;
        static raycastDelegate m_raycastImpl;
        static overlapBoxDelegate m_overlapBoxImpl;
        static overlapSphereDelegate m_overlapSphereImpl;
    };
}
