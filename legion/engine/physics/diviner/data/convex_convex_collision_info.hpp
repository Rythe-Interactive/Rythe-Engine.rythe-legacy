#pragma once
#include <core/core.hpp>
#include <physics/diviner/data/pointer_encapsulator.hpp>
#include <physics/diviner/halfedgeedge.hpp>
#include <physics/diviner/halfedgeface.hpp>

namespace legion::physics
{
    struct ConvexConvexCollisionInfo
    {
        math::float3 edgeNormal;

        float ARefSeperation, BRefSeperation, aToBEdgeSeperation;

        PointerEncapsulator < HalfEdgeFace> ARefFace;
        PointerEncapsulator < HalfEdgeFace> BRefFace;

        PointerEncapsulator< HalfEdgeEdge> edgeRef;
        PointerEncapsulator< HalfEdgeEdge> edgeInc;
    };


}
