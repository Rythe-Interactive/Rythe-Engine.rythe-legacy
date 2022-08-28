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
        math::mat4 invBindTransform;//represents the transformation from the joint to the root

        joint() = default;
        joint(id_type id, math::mat4 invBindTransform, math::mat4 animatedTransform) : id(id), invBindTransform(invBindTransform), animatedTransform(animatedTransform) {}
        joint(std::string name, id_type id, math::mat4 invBindTransform, math::mat4 animatedTransform) : id(id), name(name), invBindTransform(invBindTransform), animatedTransform(animatedTransform) {}
        ~joint() = default;

        void apply_pose(std::unordered_map<size_type, math::mat4> currentPose, math::mat4 parentTransf)
        {
            math::mat4 localTransf = currentPose[id];
            math::mat4 transf = parentTransf * localTransf;
            for (joint& j : children)
            {
                j.apply_pose(currentPose, transf);
            }
            transf = transf * invBindTransform;
            animatedTransform = transf;
        }
    };
}
