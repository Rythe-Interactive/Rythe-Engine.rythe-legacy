#pragma once
#include <core/types/types.hpp>
#include <core/math/math.hpp>
#include <core/data/joint.hpp>

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

    struct animation_clip
    {
        float length;
        std::vector<key_frame> frames;
        math::mat4 rootMat;
    };

    struct animator
    {
    public:
        animation_clip clip;

        float animTime = 0.0f;
        float progress = 0.0f;
        float frame = 0;
        key_frame previousFrame;
        key_frame nextFrame;
        std::unordered_map<size_type, math::mat4> currentPose;
    };

}
