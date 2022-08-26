#pragma once
#include <core/types/types.hpp>
#include <core/math/math.hpp>

namespace legion::core
{
    //joint-space transform, in relation to the parent joint
    struct joint_transform
    {
        math::vec3 position;
        math::quat quaternion;
    };

    struct key_frame
    {
        std::unordered_map<size_type, joint_transform> pose;
        float timeStamp;
    };

    struct animation
    {
        std::vector<key_frame> frames;
        float length;
    };

    namespace detail
    {
        joint_transform interpolate(joint_transform frameA, joint_transform frameB, float progress)
        {
            return joint_transform{ math::lerp(frameA.position,frameB.position,progress),math::slerp(frameA.quaternion,frameB.quaternion, progress) }
        }
    }

}
