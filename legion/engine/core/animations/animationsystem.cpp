#include <core/animations/animationsystem.hpp>

namespace legion::core
{
    void AnimationSystem::setup()
    {
        log::filter(log::severity_debug);
        log::debug("AnimationSystem setup");
    }

    void AnimationSystem::update(time::span deltaTime)
    {
        for (auto ent : filter)
        {
            animator& anim = ent.get_component<animator>();
            joint& rig = anim.skeleton->rootJoint;
            if (!anim.play)
                continue;
            auto clip = anim.activeClip;
            anim.time += deltaTime / 2.f;
            auto previousFrame = clip->frames[anim.frame];
            if (anim.frame == clip->frames.size() - 1)
            {
                anim.frame = -1;
                anim.time = 0;
                anim.play = false;
            }
            auto nextFrame = clip->frames[anim.frame + 1];

            float progress = (anim.time - previousFrame.timeStamp) / (nextFrame.timeStamp - previousFrame.timeStamp);
            std::unordered_map<size_type, math::mat4> currentPose;
            for (auto& [id, j_transf] : previousFrame.pose)
            {
                currentPose.emplace(id, j_transf.pose_lerp(nextFrame.pose[id], progress));
            }

            if (progress >= 1)
                anim.frame++;

            apply_pose(rig, currentPose, math::mat4(1.f));
        }
    }

    void AnimationSystem::apply_pose(joint& parentJoint, std::unordered_map<size_type, math::mat4> currentPose, math::mat4 parentTransf)
    {
        math::mat4 localTransf = currentPose[id];
        math::mat4 transf = parentTransf * localTransf;
        for (joint& j : parentJoint.children)
        {
            apply_pose(j, currentPose, transf);
        }
        parentJoint.animatedTransform = transf;
    }

    void AnimationSystem::shutdown()
    {
        log::debug("AnimationSystem shutdown");
    }
}
