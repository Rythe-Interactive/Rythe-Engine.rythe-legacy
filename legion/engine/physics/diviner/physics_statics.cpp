#include <core/core.hpp>
#include <physics/diviner/physics_statics.hpp>
#include <rendering/debugrendering.hpp>

namespace legion::physics
{
    bool PhysicsStatics::FindSeperatingAxisByExtremePointProjection(ConvexCollider* convexA
        , ConvexCollider* convexB, const math::float4x4& transformA, const math::float4x4& transformB,
        PointerEncapsulator<HalfEdgeFace>& refFace,
        float& maximumSeperation, bool shouldDebug )
    {
        float currentMaximumSeperation = std::numeric_limits<float>::lowest();

        for (auto face : convexB->GetHalfEdgeFaces())
        {
            math::float3 seperatingAxis = math::normalize(transformB * math::float4((face->normal), 0));

            math::float3 transformedPositionB = transformB * math::float4(face->centroid, 1);

            //get extreme point of other face in normal direction
            math::float3 worldSupportPoint; 
            GetSupportPointNoTransform(transformedPositionB, -seperatingAxis,
                convexA, transformA, worldSupportPoint);

            float seperation = math::dot(worldSupportPoint - transformedPositionB, seperatingAxis);

            if (seperation > currentMaximumSeperation)
            {
                currentMaximumSeperation = seperation;
                refFace.ptr = face;
            }

            if (seperation > 0)
            {
                //we have found a seperating axis, we can exit early
                maximumSeperation = currentMaximumSeperation;
                return true;
            }
        }

        //no seperating axis was found
        maximumSeperation = currentMaximumSeperation;

        return false;
    }

    float PhysicsStatics::GetSupportPoint(const std::vector<math::float3>& vertices, const math::float3& direction,math::float3& outVec)
    {
        float currentMaximumSupportPoint = std::numeric_limits<float>::lowest();

        for (const auto& vert : vertices)
        {
            float dotResult = math::dot(direction, vert);

            if (dotResult > currentMaximumSupportPoint)
            {
                currentMaximumSupportPoint = dotResult;
                outVec = vert;
            }
        }

        return currentMaximumSupportPoint;
    }

    void PhysicsStatics::GetSupportPointNoTransform(math::float3 planePosition, math::float3 direction, ConvexCollider* collider, const math::float4x4& colliderTransform
        , math::float3& worldSupportPoint)
    {
        float largestDistanceInDirection = std::numeric_limits<float>::lowest();
        planePosition = math::inverse(colliderTransform) * math::float4(planePosition, 1);
        direction = math::inverse(colliderTransform) * math::float4(direction, 0);

        const auto& vertices = collider->GetVertices();

        for (size_type vertexIndex = 0 ; vertexIndex < vertices.size(); ++vertexIndex)
        {
            math::float3 transformedVert = math::float4(vertices.at(vertexIndex), 1);

            float dotResult = math::dot(transformedVert - planePosition, direction);

            if (dotResult > largestDistanceInDirection)
            {
                largestDistanceInDirection = dotResult;
                worldSupportPoint = transformedVert;
            }
        }

        worldSupportPoint = colliderTransform * math::float4(worldSupportPoint, 1);
    }

    bool PhysicsStatics::FindSeperatingAxisByGaussMapEdgeCheck(ConvexCollider* convexA, ConvexCollider* convexB, const math::float4x4& transformA,
        const math::float4x4& transformB, PointerEncapsulator<HalfEdgeEdge>& refEdge, PointerEncapsulator<HalfEdgeEdge>& incEdge,
        math::float3& seperatingAxisFound, float& maximumSeperation, bool shouldDebug)
    {
        float currentMaximumSeperation = std::numeric_limits<float>::lowest();

        math::float3 centroidDir = transformA * math::float4(convexA->GetLocalCentroid(), 0);
        math::float3 positionA = math::float3(transformA[3]) + centroidDir;

        for (const auto faceA : convexA->GetHalfEdgeFaces())
        {
            //----------------- Get all edges of faceA ------------//
            std::vector<HalfEdgeEdge*> convexAHalfEdges;

            auto lambda = [&convexAHalfEdges](HalfEdgeEdge* edge)
            {
                convexAHalfEdges.push_back(edge);
            };

            faceA->forEachEdge(lambda);

            for (const auto faceB : convexB->GetHalfEdgeFaces())
            {
                //----------------- Get all edges of faceB ------------//
                std::vector<HalfEdgeEdge*> convexBHalfEdges;

                auto lambda = [&convexBHalfEdges](HalfEdgeEdge* edge)
                {
                    convexBHalfEdges.push_back(edge);
                };

                faceB->forEachEdge(lambda);

                for (HalfEdgeEdge* edgeA : convexAHalfEdges)
                {
                    for (HalfEdgeEdge* edgeB : convexBHalfEdges)
                    {
                        //if the given edges creates a minkowski face
                        if (attemptBuildMinkowskiFace(edgeA, edgeB, transformA, transformB))
                        {
                            //get world edge direction
                            math::float3 edgeADirection = transformA * math::float4(edgeA->getRobustEdgeDirection(), 0);
                            math::float3 edgeBDirection = transformB * math::float4(edgeB->getRobustEdgeDirection(), 0);

                            edgeADirection = math::normalize( edgeADirection );
                            edgeBDirection = math::normalize( edgeBDirection );

                            float angle = math::abs( math::dot(edgeADirection, edgeBDirection) );

                            if ( angle > physics::constants::gaussMapEdgeCheckAngleThres)
                            {
                                continue;
                            }

                            math::float3 seperatingAxis = math::normalize(math::cross(edgeADirection, edgeBDirection));

                            //get world edge position
                            math::float3 edgeAtransformedPosition = transformA * math::float4(edgeA->edgePosition, 1);
                            math::float3 edgeBtransformedPosition = transformB * math::float4(edgeB->edgePosition, 1);

                            //check if its pointing in the right direction 
                            if (math::dot(seperatingAxis, edgeAtransformedPosition - positionA) < 0)
                            {
                                seperatingAxis = -seperatingAxis;
                            }

                            //check if given edges create a seperating axis
                            float distance = math::dot(seperatingAxis, edgeBtransformedPosition - edgeAtransformedPosition);

                            if (distance > currentMaximumSeperation)
                            {
                                refEdge.ptr = edgeA;
                                incEdge.ptr = edgeB;

                                seperatingAxisFound = seperatingAxis;
                                currentMaximumSeperation = distance;
                                maximumSeperation = currentMaximumSeperation;
                            }

                            if (distance > 0)
                            {
                                return true;
                            }
                        }
                    }
                }
            }
        }

        return false;
    }

    bool PhysicsStatics::DetectConvexSphereCollision(ConvexCollider* convexA, const math::float4x4& transformA, math::float3 sphereWorldPosition, float sphereRadius,
        float& maximumSeperation)
    {
        //-----------------  check if the seperating axis is the line generated between the centroid of the hull and sphereWorldPosition ------------------//

        math::float3 worldHullCentroid = transformA * math::float4(convexA->GetLocalCentroid(), 1);
        math::float3 centroidSeperatingAxis = math::normalize(worldHullCentroid - sphereWorldPosition);

        math::float3 seperatingPlanePosition = sphereWorldPosition + centroidSeperatingAxis * sphereRadius;

        math::float3 worldSupportPoint;
        GetSupportPoint(seperatingPlanePosition, -centroidSeperatingAxis, convexA, transformA, worldSupportPoint);

        float seperation = math::dot(worldSupportPoint - seperatingPlanePosition, centroidSeperatingAxis);

        if (seperation > 0.0f)
        {
            maximumSeperation = seperation;
            return false;
        }

        maximumSeperation = std::numeric_limits<float>::lowest();

        //--------------------------------- check if the seperating axis one of the faces of the convex hull ----------------------------------------------//

        for (auto faceA : convexA->GetHalfEdgeFaces())
        {
            math::float3 worldFaceCentroid = transformA * math::float4(faceA->centroid, 1);
            math::float3 worldFaceNormal = math::normalize(transformA * math::float4(faceA->normal, 0));

            float seperation = PointDistanceToPlane(worldFaceNormal, worldFaceCentroid, seperatingPlanePosition );

            if (seperation > maximumSeperation)
            {
                maximumSeperation = seperation;
            }

            if (seperation > sphereRadius)
            {
                return false;
            }

        }


        return true;
    }

    std::pair<math::float3, math::float3> PhysicsStatics::ConstructAABBFromTransformedVertices(const std::vector<math::float3>& vertices, const math::float4x4& transform)
    {
        math::float3 min, max;
        math::float3 worldPos = transform[3];

        math::float3 outVec;
        //up
        math::float3 invTransUp = math::normalize(math::inverse(transform) * math::float4(0, 1, 0, 0));
        GetSupportPoint(vertices, invTransUp, outVec);
        max.y = (transform * math::float4( outVec,1)).y;

        //down
        math::float3 invTransDown = math::normalize(math::inverse(transform) * math::float4(0, -1, 0, 0));
        GetSupportPoint(vertices, invTransDown, outVec);
        min.y = (transform * math::float4(outVec, 1)).y;

        //right
        math::float3 invTransRight = math::normalize(math::inverse(transform) * math::float4(1, 0, 0, 0));
        GetSupportPoint(vertices, invTransRight, outVec);
        max.x = (transform * math::float4(outVec, 1)).x;

        //left
        math::float3 invTransLeft = math::normalize(math::inverse(transform) * math::float4(-1, 0, 0, 0));
        GetSupportPoint(vertices, invTransLeft, outVec);
        min.x = (transform * math::float4(outVec, 1)).x;

        //forward
        math::float3 invTransForward = math::normalize(math::inverse(transform) * math::float4(0, 0, 1, 0));
        GetSupportPoint(vertices, invTransForward, outVec);
        max.z = (transform * math::float4(outVec, 1)).z;

        //backward
        math::float3 invTransBackward = math::normalize(math::inverse(transform) * math::float4(0, 0, -1, 0));
        GetSupportPoint(vertices, invTransBackward, outVec);
        min.z = (transform * math::float4(outVec, 1)).z;

      

        return std::make_pair(min, max);
    }

    std::pair<math::float3, math::float3> PhysicsStatics::CombineAABB(const std::pair<math::float3, math::float3>& first, const std::pair<math::float3, math::float3>& second)
    {
        auto& firstLow = first.first;
        auto& firstHigh = first.second;
        auto& secondLow = second.first;
        auto& secondHigh = second.second;
        math::float3 lowBounds = secondLow;
        math::float3 highBounds = secondHigh;
        if (firstLow.x < secondLow.x)   lowBounds.x    = firstLow.x;
        if (firstLow.y < secondLow.y)   lowBounds.y    = firstLow.y;
        if (firstLow.z < secondLow.z)   lowBounds.z    = firstLow.z;
        if (firstHigh.x > secondHigh.x) highBounds.x   = firstHigh.x;
        if (firstHigh.y > secondHigh.y) highBounds.y   = firstHigh.y;
        if (firstHigh.z > secondHigh.z) highBounds.z   = firstHigh.z;

        return std::make_pair(lowBounds, highBounds);
    }

    float PhysicsStatics::FindClosestPointToLineInterpolant(const math::float3& startPoint, const math::float3& lineDirection, const math::float3& pointPosition)
    {
        return ( math::dot(lineDirection,pointPosition) - math::dot(lineDirection,startPoint) ) / math::dot(lineDirection,lineDirection);
    }

    math::float3 PhysicsStatics::FindClosestPointToLineSegment(const math::float3& start, const math::float3& end, const math::float3& pointPosition)
    {
        float interpolant = FindClosestPointToLineInterpolant(start, end - start, pointPosition);
        interpolant = math::clamp(interpolant, 0.0f, 1.0f);

        return start + (end - start) * interpolant;
    }

    std::shared_ptr<ConvexCollider> PhysicsStatics::generateConvexHull(const std::vector<math::float3>& vertices)
    {
        //[1] Calculated a scaled epsilon based on the extents of the hull. This ensures that epsilon takes the size of the hull into account.
        //[2] Calculate a visibility epsilon that determines if a vertex should be merged to the hull or not.
        //[3] Calculate the support points in x,y,z axis of the given vertices in order to create the initial hull
        //[4] Create the initial hull given the calculated support points
        //[5] Given the vector of vertices called 'vertices', partition them to the initial hull
        //[6] While there is still a face in the constructed hull with a vertex that is not merged, merge the vertex
        //[7] Populate the new ConvexCollider's face vector with the faces generated from the previous step
        //[8] Populate the new ConvexCollider's vertex vector with the vertices merged from the previous step

        //[1] Calculated a scaled epsilon based on the extents of the hull. This ensures that epsilon takes the size of the hull into account.
        const static float initialEpsilon = math::sqrt( math::epsilon<float>() );

        math::float3 maxInDimension(std::numeric_limits<float>::lowest());

        for (const auto& vert : vertices)
        {
            if (math::abs(vert.x) > maxInDimension.x)
            {
                maxInDimension.x = math::abs(vert.x);
            }

            if (math::abs(vert.y) > maxInDimension.y)
            {
                maxInDimension.y = math::abs(vert.y);
            }

            if (math::abs(vert.z) > maxInDimension.z)
            {
                maxInDimension.z = math::abs(vert.z);
            }
        }
        
        //epsilon must take into account span of vertices
        float dimensionSum = 3 * ( maxInDimension.x + maxInDimension.y + maxInDimension.z );
        const float scaledEpsilon = dimensionSum * initialEpsilon;

        //[2] Calculate a visibility epsilon that determines if a vertex should be merged to the hull or not.
        const float visibilityEpsilon = dimensionSum * math::pow(math::epsilon<float>(), 1.0f / 2.5f);

        std::vector<HalfEdgeFace*> faces;
        faces.reserve(4);

        //[3] Calculate the support points in x,y,z,-x,-y,-z axis of the given vertices in order to create the initial hull
        std::array<math::float3, 6> supportVertices;

        //Get support points in -x and x
        GetSupportPoint(vertices, math::float3(1, 0, 0), supportVertices.at(0));
        GetSupportPoint(vertices, math::float3(-1, 0, 0), supportVertices.at(1));

        //Get support points in -y and y
        GetSupportPoint(vertices, math::float3(0, 1, 0), supportVertices.at(2));
        GetSupportPoint(vertices, math::float3(0, -1, 0), supportVertices.at(3));

        //Get support points in -z and z
        GetSupportPoint(vertices, math::float3(0, 0, 1), supportVertices.at(4));
        GetSupportPoint(vertices, math::float3(0, 0, -1), supportVertices.at(5));

        //[4] Create the initial hull given the calculated support points
        if (!buildInitialHull(vertices, supportVertices, faces))
        {
            return nullptr;
        }
        
        //[5] Given the vector of vertices called 'vertices', partition them to the initial hull
        std::list<ColliderFaceToVert> facesWithOutsideVerts;
        for (HalfEdgeFace* face : faces)
        {
            facesWithOutsideVerts.emplace_back(face);
        }

        partitionVerticesToList(vertices, facesWithOutsideVerts);

        //[6] While there is still a face in the constructed hull with a vertex that is not merged, merge the vertex
        if (!facesWithOutsideVerts.empty())
        {
            PointerEncapsulator< ColliderFaceToVert> currentFaceToVert;
            currentFaceToVert.ptr = &facesWithOutsideVerts.front();
   
            while (foundFaceWithOutsideVert(facesWithOutsideVerts, currentFaceToVert))
            {
                //find furhtest vertex of last face
                auto [furthestVert, distanceFromFace] = currentFaceToVert.ptr->getFurthestOutsideVert();

                //check if we should merge this vertex
                if (distanceFromFace > visibilityEpsilon)
                {
                    mergeVertexToHull(furthestVert, facesWithOutsideVerts,scaledEpsilon);
                }
                else
                {
                    //this face has no mergable vertices
                    currentFaceToVert.ptr->outsideVerts.clear();
                }
            }
        }

        //[7] Populate the new ConvexCollider's face vector with the faces generated from the previous step
        auto convexCollider = std::make_shared<ConvexCollider>();
        auto& halfEdgesVector = convexCollider->GetHalfEdgeFaces();

        for (auto& faceToVert : facesWithOutsideVerts)
        {
            halfEdgesVector.push_back(faceToVert.face);
        }
        
        //[8] Populate the new ConvexCollider's vertex vector with the vertices merged from the previous step
        convexCollider->populateVertexListWithHalfEdges();

        return convexCollider;
    }

    void PhysicsStatics::calculateNewellPlane(const std::vector<math::float3>& v, math::float3& outPlaneNormal, float& distToCentroid)
    {
        math::float3 centroid{0,0,0};
        outPlaneNormal = math::float3();

        for (int i = v.size() - 1, j = 0; j < v.size(); i = j, j++)
        {
            outPlaneNormal.x += (v[i].y - v[j].y) * (v[i].z + v[j].z); // projection on yz
            outPlaneNormal.y += (v[i].z - v[j].z) * (v[i].x + v[j].x); // projection on xz
            outPlaneNormal.z += (v[i].x - v[j].x) * (v[i].y + v[j].y); // projection on xy
            centroid += v[j];
        }
           
        // Normalize normal and fill in the plane equation fields
        outPlaneNormal = math::normalize(outPlaneNormal);
        distToCentroid = math::dot(centroid, outPlaneNormal) / v.size(); // �centroid / n� is the true centroid point

    }

    bool PhysicsStatics::attemptBuildMinkowskiFace(HalfEdgeEdge* edgeA, HalfEdgeEdge* edgeB, const math::float4x4& transformA, const math::float4x4& transformB)
    {
        const math::float3 transformedA1 = transformA *
            math::float4(edgeA->getLocalNormal(), 0);

        const math::float3 transformedA2 = transformA *
            math::float4(edgeA->pairingEdge->getLocalNormal(), 0);

        const math::float3 transformedEdgeDirectionA =  math::normalize(transformA * math::float4(edgeA->getRobustEdgeDirection(), 0));

        const math::float3 transformedB1 = transformB *
            math::float4(edgeB->getLocalNormal(), 0);

        const math::float3 transformedB2 = transformB *
            math::float4(edgeB->pairingEdge->getLocalNormal(), 0);

        const math::float3 transformedEdgeDirectionB = math::normalize(transformB * math::float4(edgeB->getRobustEdgeDirection(), 0));

        return isMinkowskiFace(transformedA1, transformedA2, -transformedB1, -transformedB2
            , (transformedEdgeDirectionA), (transformedEdgeDirectionB));
    }

    bool PhysicsStatics::isMinkowskiFace(const math::float3& transformedA1, const math::float3& transformedA2, const math::float3& transformedB1,
        const math::float3& transformedB2, const math::float3& planeANormal, const math::float3& planeBNormal)
    {
        //------------------------ Check if normals created by arcA seperate normals of B --------------------------------------//
        
        float planeADotB1 = math::dot(planeANormal, transformedB1); //CBA
        float planeADotB2 = math::dot(planeANormal, transformedB2); //DBA

        float dotMultiplyResultA = planeADotB1 * planeADotB2;

        if (dotMultiplyResultA > 0.0f )
        {
            return false;
        }

        //------------------------ Check if normals created by arcB seperate normals of A --------------------------------------//

        float planeBDotA1 = math::dot(planeBNormal, transformedA1); //ADC
        float planeBDotA2 = math::dot(planeBNormal, transformedA2); //BDC

        float dotMultiplyResultB = planeBDotA1 * planeBDotA2;

        if (dotMultiplyResultB > 0.0f )
        {
            return false;
        }

        //------------------------ Check if arcA and arcB are in the same hemisphere --------------------------------------------//

        math::float3 abNormal = math::cross(transformedA2, transformedB2);

        float planeABDotA1 = math::dot(abNormal, transformedA1);
        float planeABDotB1 = math::dot(abNormal, transformedB1);

        float dotMultiplyResultAB = planeABDotA1 * planeABDotB1;

        if (dotMultiplyResultAB < 0.0f || math::close_enough(dotMultiplyResultAB,0.0f))
        {
            return false;
        }

        return true;
    }

    bool PhysicsStatics::buildInitialHull(const std::vector<math::float3>& vertices,
        std::array<math::float3,6>& supportVertices, std::vector<HalfEdgeFace*>& faces )
    {
        //Summary:
        //[1] Find the 2 most distant vertices in 'support Vertices'
        //[2] Find the vertex most distant from the line created by the 2 most distance vertices
        //[3] Create first collider face using that line and the vertex most distant to it
        //[4] Find the most distant vertex from the plane where the first collider face lies
        //[5] Create a set of faces connecting the first collider face to the most distant vertex,
        //this create a tetrahedron shaped collider

        //[1] Find the 2 most distant vertices in 'vertices'

        float mostDistant = std::numeric_limits<float>::lowest();

        math::float3& firstDistant = supportVertices.at(0);
        math::float3& secondDistant = supportVertices.at(1);

        //Iterate through support vertices to find the combination of vertices that represent the 2 most distant points
        for (size_type i = 0; i < supportVertices.size(); i++)
        {
            math::float3& first = supportVertices.at(i);

            for (size_type j = i+1; j < supportVertices.size()-1; j++)
            {
                math::float3& second = supportVertices.at(j);

                float currentDistance2 = math::distance2(first, second);

                if (currentDistance2 > mostDistant)
                {
                    mostDistant = currentDistance2;
                    firstDistant = first;
                    secondDistant = second;
                }
            }
        }

        //[2] Find the vertex most distant from the line created by the 2 most distance vertices

        math::float3 firstToSecond = math::normalize(secondDistant - firstDistant);

        const math::float3* thirdDistant = nullptr;
        mostDistant = std::numeric_limits<float>::lowest();

        //Iterate through 'vertices' to find the vertex most distant from line 
        for (auto& vertex : vertices)
        {
            //Check if vertex and firstDistant create a line segment parralel to the line segment created by 'firstToSecond'
            float dotResult = math::dot(math::normalize(vertex - firstDistant), firstToSecond);

            if (math::close_enough(dotResult, 1.0f))
            {
                continue;
            }

            //Find closest point between vertex and line segment
            math::float3 closestPoint = PhysicsStatics::FindClosestPointToLineSegment(firstDistant, secondDistant, vertex);

            float currentDistance = math::distance2(closestPoint, vertex);

            //Store vertex if it is further than the current known most distant
            if (currentDistance > mostDistant)
            {
                mostDistant = currentDistance;
                thirdDistant = &vertex;
            }
        }

        if (!thirdDistant)
        {
            return false;
        }

        //[3] Create first collider face using that line and the vertex most distant to it

        //Initialize Half Edges
        HalfEdgeEdge* firstEdge = new HalfEdgeEdge(firstDistant);
        HalfEdgeEdge* secondEdge = new HalfEdgeEdge(secondDistant);
        HalfEdgeEdge* thirdEdge = new HalfEdgeEdge(*thirdDistant);

        //Connect them to each other
        firstEdge->setNextAndPrevEdge(thirdEdge, secondEdge);
        secondEdge->setNextAndPrevEdge(firstEdge, thirdEdge);
        thirdEdge->setNextAndPrevEdge(secondEdge, firstEdge);

        //Initialize Half Edge Faces
        HalfEdgeFace* initialFace = new HalfEdgeFace(firstEdge,
            math::normalize(math::cross(secondDistant - firstDistant, *thirdDistant - secondDistant)));

        //Add to collider
        faces.push_back(initialFace);

        //[4] Find the most distant vertex from the plane where the first collider face lies

        //Iterate through vertices to the a point to plane check
        mostDistant = std::numeric_limits<float>::lowest();
        const math::float3* firstEyePoint = nullptr;

        math::float3 planePosition = initialFace->centroid;
        math::float3 planeNormal = initialFace->normal;

        for (auto& vertex : vertices)
        {
            float currentDistance =
                math::abs(PhysicsStatics::PointDistanceToPlane(planeNormal, planePosition, vertex));

            if (currentDistance > mostDistant)
            {
                mostDistant = currentDistance;
                firstEyePoint = &vertex;
            }
        }

        if (math::close_enough(mostDistant,0.0f))
        {
            return false;
        }

        math::color drawC = math::colors::blue;
       
        //[5] invert face if distant vertex is in front of face

        float eyePointDistance =
            PhysicsStatics::PointDistanceToPlane(planeNormal, planePosition, *firstEyePoint);
        bool needInverse = eyePointDistance > 0.0f;
        
        if (needInverse)
        {
            initialFace->inverse();
            drawC = math::colors::red;
        }

        //[5] Create a set of faces connecting the first collider face to the most distant vertex

        //Reverse collect the edges of the initialFace

        std::vector<HalfEdgeEdge*> reverseHalfEdgeList;

        auto collectEdges = [&reverseHalfEdgeList](HalfEdgeEdge* current)
        {
            reverseHalfEdgeList.push_back(current);
        };

        initialFace->forEachEdgeReverse(collectEdges);

        //For each edge create a new face that connects the initial face with the eyePoint
        math::float3 eyePoint = *firstEyePoint;

        createHalfEdgeFaceFromEyePoint(eyePoint, reverseHalfEdgeList,faces);

        return true;
    }

    void PhysicsStatics::createHalfEdgeFaceFromEyePoint(const math::float3 eyePoint, const std::vector<HalfEdgeEdge*>& reversedEdges, std::vector<HalfEdgeFace*>& createdFaces)
    {
        HalfEdgeEdge* pairingToConnectTo = nullptr;
        HalfEdgeEdge* initialPairing = nullptr;

        for (auto edge : reversedEdges)
        {
            //initialize pairing its position is on next
            HalfEdgeEdge* pairing = new HalfEdgeEdge(edge->nextEdge->edgePosition);

            //initialize next pairing its position is on current
            HalfEdgeEdge* nextPairing = new HalfEdgeEdge(edge->edgePosition);

            //initialize prev pairing its position is on the eye point
            HalfEdgeEdge* prevPairing = new HalfEdgeEdge(eyePoint);

            //connect to each other
            pairing->setNextAndPrevEdge(prevPairing, nextPairing);
            nextPairing->setNextAndPrevEdge(pairing, prevPairing);
            prevPairing->setNextAndPrevEdge(nextPairing, pairing);

            //
            pairing->setPairingEdge(edge);

            //initialize new face
            math::float3 faceNormal = math::normalize(math::cross(nextPairing->edgePosition - pairing->edgePosition,
                prevPairing->edgePosition - pairing->edgePosition));

            HalfEdgeFace* face = new HalfEdgeFace(pairing, faceNormal);

            //push new face into list 
            createdFaces.push_back(face);

            //connect to 
            if (pairingToConnectTo)
            {
                prevPairing->setPairingEdge(pairingToConnectTo);
            }
            else
            {
                initialPairing = prevPairing;
            }

            pairingToConnectTo = nextPairing;
        }

        initialPairing->setPairingEdge(pairingToConnectTo);
    }

    bool PhysicsStatics::foundFaceWithOutsideVert(std::list<ColliderFaceToVert>& facesWithOutsideVerts, PointerEncapsulator< ColliderFaceToVert>& outChosenFace)
    {
        for (auto& faceWithOutsideVert : facesWithOutsideVerts)
        {
            if (!faceWithOutsideVert.outsideVerts.empty())
            {
                outChosenFace.ptr = &faceWithOutsideVert;
                return true;
            }
        }

        return false;
    }

    void PhysicsStatics::partitionVerticesToList(const std::vector<math::float3> vertices,
        std::list<ColliderFaceToVert>& outFacesWithOutsideVerts)
    {
        for (const math::float3& vertex : vertices)
        {
            ColliderFaceToVert* bestFaceToVert = nullptr;
            float bestMatchDistance = std::numeric_limits<float>::lowest();

            //find the face that is most distant from the std::list of planes given by 'outFacesWithOutsideVerts'
            for (ColliderFaceToVert& faceToVert : outFacesWithOutsideVerts)
            {
                auto face = faceToVert.face;
                float currentDistance = PointDistanceToPlane(face->normal, face->centroid, vertex);

                if (currentDistance > bestMatchDistance)
                {
                    bestMatchDistance = currentDistance;
                    bestFaceToVert = &faceToVert;
                }
            }

            if (bestFaceToVert && bestMatchDistance > 0.0f)
            {
                bestFaceToVert->outsideVerts.push_back(vertex);
            }
        }
    }

    void PhysicsStatics::findHorizonEdgesFromFaces(const math::float3& eyePoint, std::vector<HalfEdgeFace*>& faces,
        std::vector<HalfEdgeEdge*>& outHorizonEdges, float scalingEpsilon)
    {
        //[1] Find first horizon edge
        HalfEdgeEdge* initialHorizon = nullptr;
        
        auto findFirstHorizon = [&eyePoint,&initialHorizon,scalingEpsilon](HalfEdgeEdge*edge)
        {
            if (initialHorizon) { return; }
           
            if (edge->isEdgeHorizonFromVertex(eyePoint, scalingEpsilon))
            {
                initialHorizon = edge;
            }
        };

        for (auto face : faces)
        {
            if (initialHorizon) { break; }

            face->forEachEdge(findFirstHorizon);
            
        }

        assert(initialHorizon);

        //[2] Loop through the collider to collect the other horizon edges
       
        HalfEdgeEdge* currentEdge = initialHorizon;

        do
        {
            currentEdge = currentEdge->nextEdge;

            if (!currentEdge->isEdgeHorizonFromVertex(eyePoint, scalingEpsilon))
            {
                auto stuckEdge = currentEdge;
                do
                {
                    auto pairing = currentEdge->pairingEdge;
                    currentEdge = currentEdge->pairingEdge->nextEdge;
                }
                while (!currentEdge->isEdgeHorizonFromVertex(eyePoint, scalingEpsilon));
            }

            outHorizonEdges.push_back(currentEdge);

        } while (currentEdge != initialHorizon);

    }

    void PhysicsStatics::mergeVertexToHull(const math::float3& eyePoint,std::list<ColliderFaceToVert>& facesWithOutsideVerts,
        float scalingEpsilon)
    {
        //[1] identify faces that can see the 'eyePoint' and remove them from list
        //[2] Find the horizon edges. These are the edges that neighbor a face that can see the eyePoint
        //and a face that cannot
        //[3] Given the horizon edges and the eye point. Create a new faces that merge the eyepoint
        //[4] Merge all new faces until no coplanarity/concavity can be detected
        //[5] Remove duplicate faces are generated while face merging
        //[6] Repartition vertices that were placed in the unmergedVertices vector.
        //[7] Delete all faces that were removed at step [1]

        std::vector<math::float3> unmergedVertices;
        std::vector<HalfEdgeFace*> facesToBeRemoved;
          
        //[1] identify faces that can see the 'eyePoint' and remove them from list
        for (auto listIter = facesWithOutsideVerts.begin(); listIter != facesWithOutsideVerts.end();)
        {
            HalfEdgeFace* face = listIter->face;

            const math::float3& planeCentroid = face->centroid;
            const math::float3& planeNormal = face->normal;

            float distanceToPlane = PointDistanceToPlane(planeNormal, planeCentroid, eyePoint);

            if (distanceToPlane > scalingEpsilon)
            {
                //face can see vertex, we must remove it from list
                facesToBeRemoved.push_back(face);
                listIter->populateVectorWithVerts(unmergedVertices);
                listIter = facesWithOutsideVerts.erase(listIter);
            }
            else
            {
                listIter++;
            }
        }
        
        //[2] Find the horizon edges. These are the edges that neighbor a face that can see the eyePoint
        //and a face that cannot
        std::vector<HalfEdgeEdge*> horizonEdges;
        findHorizonEdgesFromFaces(eyePoint, facesToBeRemoved, horizonEdges,scalingEpsilon);

        //reverse iterate the list to find their pairings, add them to new list
        {
            std::vector<HalfEdgeEdge*> tempEdges = std::move(horizonEdges);
            horizonEdges.clear();

            //what we actually want is its pairings
            for (auto horizonEdge : tempEdges)
            {
                horizonEdges.push_back(horizonEdge->pairingEdge);
            }
        }

        //[3] Given the horizon edges and the eye point. Create a new faces that merge the eyepoint
        std::vector<HalfEdgeFace*> newFaces;
        newFaces.reserve(horizonEdges.size());
        createHalfEdgeFaceFromEyePoint(eyePoint, horizonEdges, newFaces);

        //[4] Merge all new faces until no coplanarity/concavity can be detected
        auto oldEndIter = facesWithOutsideVerts.end();
        oldEndIter--;

        for (HalfEdgeFace* face : newFaces)
        {
            facesWithOutsideVerts.emplace_back(face);
        }

        for (auto listIter = oldEndIter; listIter != facesWithOutsideVerts.end(); listIter++)
        {
            bool mergedInIteration = false;
            if (!listIter->face) {  continue; }

            do
            {
                mergedInIteration = false;

                std::vector<HalfEdgeEdge*> faceEdges;
                faceEdges.reserve(3);

                auto edgeCollect = [&faceEdges](HalfEdgeEdge* edge){ faceEdges.push_back(edge); };

                listIter->face->forEachEdge(edgeCollect);

                for (auto edge : faceEdges)
                {
                    HalfEdgeEdge* pairingEdge = edge->pairingEdge;
                    HalfEdgeFace* currentFace = edge->face;

                    math::float3 newNormal;

                    if (isNewellFacesCoplanar(currentFace, pairingEdge->face, edge,
                        scalingEpsilon, newNormal,1) || isFacesConcave(edge->face, pairingEdge->face))
                    {
                        edge->suicidalMergeWithPairing(unmergedVertices,newNormal,scalingEpsilon);

                        mergedInIteration = true;
                        break;
                    }
                    else
                    {
                        HalfEdgeEdge* pairing = edge->pairingEdge;
                        HalfEdgeEdge* nextPairing = edge->nextEdge->pairingEdge;

                        //if this face a pair of adjacent edges that connect with the same face but they are not coplanar,
                        //we essentially 'extend' the first edge and delete the other. We do the opposite for
                        //its neighboring edge pair
                        if (pairing->face == nextPairing->face)
                        {
                            HalfEdgeEdge* oldNextEdge = edge->nextEdge;
                            edge->setNext(oldNextEdge->nextEdge);

                            HalfEdgeEdge* oldPairingNextEdge = nextPairing->nextEdge;
                            nextPairing->setNext(oldPairingNextEdge->nextEdge);

                            edge->setPairingEdge(nextPairing);

                            edge->face->startEdge = edge;
                            edge->pairingEdge->face->startEdge = nextPairing;

                            delete oldNextEdge;
                            delete oldPairingNextEdge;

                            mergedInIteration = true;
                            break;
                        }
                    }
                }
            } while (mergedInIteration);
        }

        //[5] Remove duplicate faces are generated while face merging
        std::set<HalfEdgeFace*> uniqueFaces;
        for (auto listIter = facesWithOutsideVerts.begin(); listIter != facesWithOutsideVerts.end();)
        {
            if (!listIter->face ) { listIter = facesWithOutsideVerts.erase(listIter); continue; } 
            auto iter = uniqueFaces.find(listIter->face);

            if (iter != uniqueFaces.end())
            {
                //this face is a duplicate
                listIter->populateVectorWithVerts(unmergedVertices);
                listIter = facesWithOutsideVerts.erase(listIter);
            }
            else
            {
                //we have not seen this face before
                uniqueFaces.insert(listIter->face);
                listIter++;
            }
        }

        //[6] Repartition vertices that were placed in the unmergedVertices vector.
        partitionVerticesToList(unmergedVertices, facesWithOutsideVerts);

        //[7] Delete all faces that were removed at step [1]
        for (auto face : facesToBeRemoved)
        {
            delete face;
        }
    }

    bool PhysicsStatics::isFacesConcave(HalfEdgeFace* first, HalfEdgeFace* second)
    {
        float dotResult = math::dot( first->normal, second->centroid - first->centroid );
        return dotResult > 0.0;
    }

    bool PhysicsStatics::isNewellFacesCoplanar(HalfEdgeFace* first, HalfEdgeFace* second,
        HalfEdgeEdge* connectingEdge, float scalingEpsilon,  math::float3& outNormal,int skipCount)
    {
        //each face will at least have 3 vertices
        std::vector<math::float3> NewellPolygon;
        NewellPolygon.reserve(6);

        auto firstOriginal = first->startEdge;
        auto secondOriginal = second->startEdge;

        assert(first == connectingEdge->face);

        first->startEdge = connectingEdge; 
        second->startEdge = connectingEdge->pairingEdge;

        for (size_t i = 0; i < skipCount; i++)
        {
            first->startEdge = first->startEdge->nextEdge;
            second->startEdge = second->startEdge->nextEdge;
        }

        auto collectVerticesOfFace = [&NewellPolygon](HalfEdgeEdge* edge)
        {
            NewellPolygon.emplace_back(edge->edgePosition );
        };

        first->forEachEdge(collectVerticesOfFace);
        NewellPolygon.pop_back();

        second->forEachEdge(collectVerticesOfFace);
        NewellPolygon.pop_back();

         float distToCentroid;
        calculateNewellPlane(NewellPolygon, outNormal, distToCentroid);

        float epsilonMultiplier = (first->calculateFaceExtents() + second->calculateFaceExtents()) * 0.5f;

        for (int i = 0; i < NewellPolygon.size(); i++) {
            float dist = math::dot(outNormal, NewellPolygon[i]) - distToCentroid;

            if (math::abs(dist) > scalingEpsilon * epsilonMultiplier)
            {
                return false;
            }
        }

        return true;
    }
};
