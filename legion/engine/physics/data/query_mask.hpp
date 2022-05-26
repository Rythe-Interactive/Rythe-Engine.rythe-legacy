#pragma once
#include <core/core.hpp>
#include <bitset>
#include <physics/data/component_flags.hpp>

namespace legion::physics
{
    class QueryMask
    {
    public:

        QueryMask() = default;

        QueryMask(physics_object_reaction reaction);

        void setReactionToMaskIndex(size_type maskIndex, physics_object_reaction reaction);

        physics_object_reaction getReactionToMask(size_type maskIndex) const;

        void setReactionToDefaultMask(physics_object_reaction reaction)
        {
            m_defaultReaction = reaction;
        }

        physics_object_reaction getDefaultReaction() const noexcept { return m_defaultReaction; }

        std::uint32_t getMask() const noexcept { return m_mask; }

    private:

        std::uint32_t m_mask = std::numeric_limits< std::uint32_t>::max();
        physics_object_reaction m_defaultReaction = physics_object_reaction::reaction_block;
    };

}
