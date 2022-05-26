#pragma once
#include <core/core.hpp>

namespace legion::physics
{
    struct QueryHitInfo
    {
        ecs::entity entityHit;
        math::vec3 position;
        math::vec3 normal;
    };

    struct QueryHit
    {
        QueryHitInfo blockingHit;
        bool blockFound;
        std::vector<QueryHitInfo> touchingHit;
    };

    struct OverlapHitInfo
    {
        ecs::entity entityOverlapped;
    };

    struct OverlapHit
    {
        std::vector<OverlapHitInfo> overlapHits;
    };
}
