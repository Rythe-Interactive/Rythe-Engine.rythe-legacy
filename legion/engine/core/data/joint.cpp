#include <core/data/joint.hpp>

namespace legion::core
{
    void joint::calc_inverse_bind_transf(math::mat4 parentBindTransf)
    {
        math::mat4 bindTransf = parentBindTransf * localBindTransform;
        invBindTransform = math::inverse(bindTransf);
        for (joint& j : children)
        {
            j.calc_inverse_bind_transf(bindTransf);
        }
    }

    void joint::apply_pose(std::unordered_map<size_type, math::mat4> currentPose, math::mat4 parentTransf)
    {
        math::mat4 localTransf = currentPose[id];
        math::mat4 transf = parentTransf * localTransf;
        for (joint& j : children)
        {
            j.apply_pose(currentPose, transf);
        }
        animatedTransform = transf;
    }

    void joint::apply_pose(std::vector<math::mat4> currentPose, math::mat4 parentTransf)
    {
        math::mat4 localTransf = currentPose[id];
        math::mat4 transf = parentTransf * localTransf;
        for (joint& j : children)
        {
            j.apply_pose(currentPose, transf);
        }
        animatedTransform = transf;
    }

    std::vector<math::mat4> joint::get_joint_transforms()
    {
        std::vector<math::mat4> transforms;
        transforms.resize(get_max_id()+1);
        get_joint_internal(transforms);
        return transforms;
    }

    void joint::get_joint_internal(std::vector<math::mat4>& list)
    {
        list.at(id) = animatedTransform;
        for (auto& j : children)
            j.get_joint_internal(list);
    }

    id_type joint::get_max_id()
    {
        id_type max = id;
        for (auto& j : children)
            max = math::max(max, j.get_max_id());

        return max;
    }
}
