#pragma once
#include <physx/PxPhysicsAPI.h>
#include <physics/physics_helpers.hpp>

using namespace physx;

namespace legion::physics
{
    physics_object_reaction getQueryReaction(PxU32 reactionList, int otherObjectType)
    {
        physics_object_reaction result = physics_object_reaction::reaction_ignore;

        size_t queryReactionMax = physics_object_reaction::reaction_max - 1;

        for (size_t i = queryReactionMax * otherObjectType, j = 0; j < queryReactionMax; i++, j++)
        {
            if ((reactionList >> i) & 1)
            {
                return static_cast<physics_object_reaction>(j + 1);
            }
        }

        return result;
    }

    class QueryFilterCallback : public PxQueryFilterCallback
    {
        PxQueryHitType::Enum preFilter(
            const PxFilterData& rayFilterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags) override
        {
            ecs::entity ent = { static_cast<ecs::entity_data*>(actor->userData) };
            
            //get filter data of shape
            const PxFilterData objectFilter = shape->getQueryFilterData();

            //immediately return if ray is default
            if (rayFilterData.word0 == std::numeric_limits<PxU32>::max())
            {
                return PxQueryHitType::eBLOCK;
            }
            //if object has default mask use ray default reaction instead
            if (objectFilter.word2 == std::numeric_limits<PxU32>::max())
            {
                if (rayFilterData.word1 == physics_object_reaction::reaction_block)
                {
                    return PxQueryHitType::eBLOCK;
                }
                else if (rayFilterData.word1 == physics_object_reaction::reaction_overlap)
                {
                    return PxQueryHitType::eTOUCH;
                }
                else
                {
                    return PxQueryHitType::eNONE;
                }
            }

            PxU32 objectType = objectFilter.word2;

            //check reaction of ray towards mask
            physics_object_reaction reactToRay = getQueryReaction(rayFilterData.word0, objectType);

            if (reactToRay == physics_object_reaction::reaction_block)
            {
                return PxQueryHitType::eBLOCK;
            }
            else if (reactToRay == physics_object_reaction::reaction_overlap)
            {
                return PxQueryHitType::eTOUCH;
            }

            //return respective reaction
            return PxQueryHitType::eNONE;
        }

        virtual PxQueryHitType::Enum postFilter(const PxFilterData& filterData, const PxQueryHit& hit)
        {
            return PxQueryHitType::eNONE;
        }
    };

    template<class HitBuffer>
    static void populateQueryHit(const HitBuffer* buffer, PxU32 nbHits, pointer<QueryHit> queryHit)
    {
        static_assert(std::is_base_of<PxLocationHit,HitBuffer>::value,
            "template parameter of ::populateQuery must inherit from PxLocationHit");

        queryHit->touchingHit.reserve(nbHits);

        for (size_t i = 0; i < nbHits; i++)
        {
            ecs::entity ent = { static_cast<ecs::entity_data*>(buffer[i].actor->userData) };

            PxVec3 pxPos = buffer[i].position;
            PxVec3 pxNorm = buffer[i].normal;

            math::vec3 position{ pxPos.x,pxPos.y,pxPos.z };
            math::vec3 normal{ pxNorm.x,pxNorm.y,pxNorm.z };

            queryHit->touchingHit.emplace_back(QueryHitInfo{ ent,position,normal });
        }
    }

    struct RaycastCallback : public PxRaycastCallback
    {
        RaycastCallback(pointer<QueryHit> raycastHitPtr,PxRaycastHit* aTouches, PxU32 aMaxNbTouches) :
            PxRaycastCallback(aTouches, aMaxNbTouches), queryHit{raycastHitPtr} { }

        PxAgain processTouches(const PxRaycastHit* buffer, PxU32 nbHits) override
        {
            populateQueryHit<PxRaycastHit>(buffer, nbHits, queryHit);

            return false;
        }

        pointer<QueryHit> queryHit;
    };

    struct SweepCallback : public PxSweepCallback
    {
        SweepCallback(pointer<QueryHit> raycastHitPtr, PxSweepHit* aTouches, PxU32 aMaxNbTouches) :
            PxSweepCallback(aTouches, aMaxNbTouches), queryHit{ raycastHitPtr } { }

        PxAgain processTouches(const PxSweepHit* buffer, PxU32 nbHits) override
        {
            populateQueryHit<PxSweepHit>(buffer, nbHits, queryHit);

            return false;
        }

        pointer<QueryHit> queryHit;
    };

    struct OverlapCallback : public PxOverlapCallback
    {
        OverlapCallback(pointer<OverlapHit> raycastHitPtr, PxOverlapHit* aTouches, PxU32 aMaxNbTouches) :
            PxOverlapCallback(aTouches, aMaxNbTouches), queryHit{ raycastHitPtr } { }

        PxAgain processTouches(const PxOverlapHit* buffer, PxU32 nbHits) override
        {
            queryHit->overlapHits.reserve(nbHits);

            for (size_t i = 0; i < nbHits; i++)
            {
                ecs::entity overlapEnt  = { static_cast<ecs::entity_data*>(buffer[i].actor->userData) };
                queryHit->overlapHits.emplace_back(OverlapHitInfo{ overlapEnt });
            }

            return false;
        }

        pointer<OverlapHit> queryHit;
    };

}
