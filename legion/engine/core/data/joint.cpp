#include <core/data/joint.hpp>

namespace legion::core
{
    void joint::apply_pose(std::unordered_map<size_type, math::mat4> currentPose, math::mat4 parentTransf)
    {
        math::mat4 localTransf = currentPose[id];
        math::mat4 transf = parentTransf * localTransf;
        for (joint& j : children)
        {
            j.apply_pose(currentPose, transf);
        }
        //transf = transf * invBindTransform;
        animatedTransform = transf;
    }
}
