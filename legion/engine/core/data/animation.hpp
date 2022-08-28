#pragma once
#include <core/types/types.hpp>
#include <core/math/math.hpp>

namespace legion::core
{
    struct joint_transform
    {
        math::vec3 translation;
        math::quat rotation;
        math::vec3 scale;

        math::mat4 pose_lerp(joint_transform target, float progress)
        {
            math::vec3 pos = math::lerp(translation, target.translation, progress);
            math::quat rot = math::slerp(rotation, target.rotation, progress);
            math::vec3 scal = math::lerp(scale, target.scale, progress);
            return math::compose(scal, rot, pos);
        }
    };

    struct key_frame
    {
        float timeStamp;
        std::unordered_map<size_type, joint_transform> pose;
    };

    struct animation
    {
        std::vector<key_frame> frames;
        float length;
    };

}
