#pragma once
#include <core/types/types.hpp>
#include <core/math/math.hpp>
#include <core/assets/assets.hpp>

namespace legion::core
{
    struct joint
    {
        id_type id;
        id_type idOffset;
        std::string name;
        std::vector<joint> children;
        math::mat4 animatedTransform;//represents a joints transformation from old to new position
        math::mat4 localBindTransform;//represents the transform from the root to the joint
        math::mat4 invBindTransform;//represents the transformation from the joint to the root

        joint() = default;
        joint(id_type id, math::mat4 localBindTransform, math::mat4 animatedTransform) : id(id), localBindTransform(localBindTransform), animatedTransform(animatedTransform) {}
        joint(std::string name, id_type id, math::mat4 localBindTransform, math::mat4 animatedTransform) : id(id), name(name), localBindTransform(localBindTransform), animatedTransform(animatedTransform) {}
        ~joint() = default;

        void calc_inverse_bind_transf(math::mat4 parentBindTransf);
        void set_inv_bind_mats(std::vector<math::mat4>& invBindMats);

        void apply_pose(std::unordered_map<size_type, math::mat4> currentPose, math::mat4 parentTransf);
        std::vector<math::mat4> get_joint_transforms();
        id_type get_max_id();
        void get_joint_internal(std::vector<math::mat4>& list);
    };

    struct skeleton
    {
        joint rootJoint;
        std::vector<math::mat4> jointTransforms;
    };

    ReportAssetType(skeleton);
}
