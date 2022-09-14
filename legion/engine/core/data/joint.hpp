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
        math::mat4 localBindTransform;//represents the transform from the root to the joint
        math::mat4 invBindTransform;//represents the transformation from the joint to the root

        joint() = default;
        joint(id_type id, math::mat4 localBindTransform, math::mat4 animatedTransform) : id(id), localBindTransform(localBindTransform), animatedTransform(animatedTransform) {}
        joint(std::string name, id_type id, math::mat4 localBindTransform, math::mat4 animatedTransform) : id(id), name(name), localBindTransform(localBindTransform), animatedTransform(animatedTransform) {}
        ~joint() = default;

        void calcInverseBindTransform(math::mat4 parentBindTransf);

        void apply_pose(std::unordered_map<size_type, math::mat4> currentPose, math::mat4 parentTransf);
    };

    struct skeleton
    {
        joint rootJoint;
    };
}
