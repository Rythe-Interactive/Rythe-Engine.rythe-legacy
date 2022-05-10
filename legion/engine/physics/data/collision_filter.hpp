#pragma once
#include <core/core.hpp>
#include <bitset>
#include <physics/data/component_flags.hpp>

namespace legion::physics
{
    class CollisionFilter
    {
    public:

        CollisionFilter()
        {
            for (size_t blockBit = physics_object_reaction::reaction_block;
                blockBit < m_bitsetArray.size(); blockBit += physics_object_reaction::reaction_max)
            {
                m_bitsetArray.set(blockBit);
            }
        }

        void setReactionToObject(physics_object_flag objectType, physics_object_reaction newReaction)
        {
            for (size_t reactionIndex = 0; reactionIndex < physics_object_reaction::reaction_max; reactionIndex++)
            {
                m_bitsetArray.set(objectType * physics_object_reaction::reaction_max + reactionIndex,false);
            }

            m_bitsetArray.set(objectType * physics_object_reaction::reaction_max + newReaction);
        }

        physics_object_reaction getReaction(physics_object_flag objectType)
        {
            for (size_t reactionIndex = 0; reactionIndex < physics_object_reaction::reaction_max; reactionIndex++)
            {
                if (m_bitsetArray.test(objectType * physics_object_reaction::reaction_max + reactionIndex))
                {
                    return static_cast<physics_object_reaction>(reactionIndex);
                }
            }

            return physics_object_reaction::reaction_max;//should never get here
        }

        L_ALWAYS_INLINE auto getBitset() const noexcept { return m_bitsetArray; }

    private:

        std::bitset<physics_object_reaction::reaction_max * physics_object_flag::po_max> m_bitsetArray;
    };
}
