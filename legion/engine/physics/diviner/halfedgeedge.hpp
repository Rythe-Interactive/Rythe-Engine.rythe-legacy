#pragma once
#include <core/core.hpp>
#include <physics/diviner/data/edge_label.hpp>

#include <rendering/debugrendering.hpp>

namespace legion::physics
{
    struct HalfEdgeFace;

    struct HalfEdgeEdge
    {
        HalfEdgeEdge* pairingEdge = nullptr;
        HalfEdgeEdge* nextEdge = nullptr;
        HalfEdgeEdge* prevEdge = nullptr;

        HalfEdgeFace* face = nullptr;

        EdgeLabel label;

        math::float3 edgePosition;
        math::float3 robustEdgeDirection;
        std::string id;

        HalfEdgeEdge() = default;

        HalfEdgeEdge(math::float3 newEdgePositionPtr);

        /**@brief sets 'nextEdge' and 'prevEdge' with the given HalfEdgeEdges
         */
        void setNextAndPrevEdge(HalfEdgeEdge* newPrevEdge, HalfEdgeEdge* newNextEdge);

        /**@brief sets 'nextEdge' of this edge to 'newNextEdge' and sets the 'prevEdge' of 'newNextEdge' to this edge
         */
        void setNext(HalfEdgeEdge* newNextEdge);

        /**@brief sets 'prevEdge' of this edge to 'newPrevEdge' and sets the 'nextEdge' of 'newPrevEdge' to this edge
         */
        void setPrev(HalfEdgeEdge* newPrevEdge);

        /**@brief Sets the pairingEdge to the passed in edge, and sets the edge.pairingEdge to this
         * if this edge or the passed edge already have pairing edge, the pairing edge of the pairing edge will be set to nullptr
         */
        void setPairingEdge(HalfEdgeEdge* edge);

        /**@brief Gets the normal of the face of this edge
         */
        math::float3 getLocalNormal() const;

        /**@brief Gets the precalculated direction of this edge */
        math::float3 getRobustEdgeDirection() const { return robustEdgeDirection; }

        /**@brief Calculates the direction of this edge based on its the 2 adjacent faces of this edge */
        void calculateRobustEdgeDirection();

        /**@brief Checks if the face of this edge is below 'vert' with a distance given by 'epsilon'
        */
        bool isVertexVisible(const math::float3& vert, float epsilon = math::epsilon_v<float>);

        /**@brief Checks if the face of this edge is below 'vert' with a distance given by 'epsilon'
         * while this edges neighboring face is above it
         */
        bool isEdgeHorizonFromVertex(const math::float3& vert,float epsilon = math::epsilon_v<float>);

        /**@brief Merges this edges face with the face of this edge's pairing. The latter face, this edge, and its pairing edge
         * is destroyed in the process. 
         */
        void suicidalMergeWithPairing(std::vector<math::float3>& unmergedVertices, math::float3& normal, float scalingEpsilon);
       
        void DEBUG_drawEdge(const math::float4x4& transform, const math::color& debugColor, float time = 20.0f, float width = 5.0f);
        
        void DEBUG_drawInsetEdge(const math::float3 spacing, const math::color& debugColor, float time = 20.0f, float width = 5.0f);

        void DEBUG_directionDrawEdge(const math::float4x4& transform, const math::color& debugColor, float time, float width);
    };
}

