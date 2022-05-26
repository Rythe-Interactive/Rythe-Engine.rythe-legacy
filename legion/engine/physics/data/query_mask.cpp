#include "query_mask.hpp"

namespace legion::physics
{
    QueryMask::QueryMask(physics_object_reaction reaction)
    {
        setReactionToDefaultMask(reaction);

        std::uint32_t maskCount = sizeof(std::uint32_t) * 8 / 2;

        for (size_t i = 0; i < maskCount; i++)
        {
            setReactionToMaskIndex(i, reaction);
        }

    }
    void QueryMask::setReactionToMaskIndex(size_type maskIndex, physics_object_reaction reaction)
    {
        if (m_mask == std::numeric_limits<std::uint32_t>::max()) { m_mask = 0; }

        //reset mask for index
        int overlapReactionBit = (maskIndex * (physics_object_reaction::reaction_max - 1));

        m_mask &= ~(1 << overlapReactionBit);
        m_mask &= ~(1 << (overlapReactionBit +1));

        if (reaction == physics_object_reaction::reaction_ignore) return;

        int reactionOffset = reaction - 1;

        int newReactionMask = 1 << (overlapReactionBit + reactionOffset);
        m_mask |= newReactionMask;
    }

    physics_object_reaction QueryMask::getReactionToMask(size_type maskIndex) const
    {
        size_type offset = (physics_object_reaction::reaction_max - 1);

        for (size_t i = 0; i < offset; i++)
        {
            if (m_mask >> (maskIndex * offset) & 1)
            {
                return static_cast<physics_object_reaction>(i + 1);
            }
        }

        return physics_object_reaction::reaction_ignore;
    }
}
