#pragma once
#include <core/types/types.hpp>
#include <core/math/math.hpp>

namespace legion::core
{
    struct joint
    {
        id_type id;
        std::string name;
        std::vector<joint> children;
        math::mat4 animatedTransform;//represents a joints transformation from old to new position

        math::mat4 localBindTransform;//represents a joints transformation from its parent
        math::mat4 invBindTransform;//represents the transformation from the joint to the root


        inline void calcInverseBindTransform(math::mat4 parentBindTransform)
        {
            math::mat4 bindTransform = parentBindTransform * localBindTransform;
            invBindTransform = math::inverse(bindTransform);
            for (joint child : children)
                child.calcInverseBindTransform(bindTransform);
        }
    };
}
