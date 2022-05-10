#include <physx/PxPhysicsAPI.h>
#include <physics/data/component_flags.hpp>

namespace legion::physics
{
    physics_object_reaction getReaction(PxU32 reactionList, PxU32 otherObjectTye)
    {
        physics_object_reaction result = physics_object_reaction::reaction_ignore;

        for (size_t currentBitPosition = physics_object_reaction::reaction_max * otherObjectTye, objectReactionIndex = 0;
            objectReactionIndex < physics_object_reaction::reaction_max; currentBitPosition++, objectReactionIndex++)
        {
            if ((reactionList >> currentBitPosition) & 1)
            {
                return static_cast<physics_object_reaction>(objectReactionIndex);
            }
        }

        return result;
    }

    PxFilterFlags filterShader(
        PxFilterObjectAttributes attributes0, PxFilterData filterData0,
        PxFilterObjectAttributes attributes1, PxFilterData filterData1,
        PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
    {
        PxU32 firstObjType = filterData0.word1;
        PxU32 secondObjType = filterData1.word1;

        //get reaction of objects to each other
        physics_object_reaction firstToSecond = getReaction(filterData0.word0, secondObjType);
        physics_object_reaction secondToFirst = getReaction(filterData1.word0, firstObjType);

        physics_object_reaction lowestReaction = firstToSecond < secondToFirst ? firstToSecond : secondToFirst;

        if (lowestReaction == physics_object_reaction::reaction_block)
        {
            pairFlags = PxPairFlag::eCONTACT_DEFAULT;
        }
        else if (lowestReaction == physics_object_reaction::reaction_overlap)
        {
            pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
        }
        else
        {
            return PxFilterFlag::eSUPPRESS;
        }

        return PxFilterFlag::eDEFAULT;
    }

    class CustomControllerFilterCallback : public PxQueryFilterCallback
    {
        PxQueryHitType::Enum preFilter(
            const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags)
        {
            PxFilterData otherData = shape->getQueryFilterData();

            physics_object_reaction characterControllerToShape = getReaction(filterData.word0, otherData.word3);
            physics_object_reaction shapeToCharacterController = getReaction(otherData.word0, physics_object_flag::po_character_controller);

            physics_object_reaction lowestReaction = characterControllerToShape < shapeToCharacterController ? characterControllerToShape : shapeToCharacterController;

            if (lowestReaction == physics_object_reaction::reaction_block)
            {
                return PxQueryHitType::eTOUCH;
            }

            return PxQueryHitType::eNONE;
        }

        virtual PxQueryHitType::Enum postFilter(const PxFilterData& filterData, const PxQueryHit& hit)
        {
            return PxQueryHitType::eNONE;
        }
    };

}
