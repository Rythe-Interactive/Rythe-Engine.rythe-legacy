#include <core/data/joint.hpp>

namespace legion::core
{
    void joint::calcInverseBindTransform(math::mat4 parentBindTransf)
    {
        math::mat4 bindTransf = parentBindTransf * localBindTransform;
        invBindTransform = math::inverse(bindTransf);
        for (joint& j : children)
        {
            j.calcInverseBindTransform(bindTransf);
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
}
