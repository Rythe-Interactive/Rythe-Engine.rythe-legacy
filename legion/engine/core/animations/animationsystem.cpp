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
            auto riggedMesh = ent.get_component<mesh_filter>()->shared_mesh;
            auto& rig = riggedMesh->skeleton.rootJoint;
            auto anim = ent.get_component<animator>();
            auto clip = anim->clip;

            float& animTime = anim->animTime;
            animTime += deltaTime;

            float length = clip.length;
            if (animTime > length)
                animTime = 0;

            auto& previousFrame = anim->previousFrame;
            previousFrame = clip.frames[anim->frame];
            auto& nextFrame = anim->nextFrame;
            nextFrame = clip.frames[anim->frame == clip.frames.size() - 1 ? 0 : anim->frame + 1];

            float& progress = anim->progress;
            progress = (animTime - previousFrame.timeStamp) / (nextFrame.timeStamp - previousFrame.timeStamp);

            auto& currentPose = anim->currentPose;

            for (auto& [id, j_transf] : previousFrame.pose)
            {
                currentPose.emplace(id, j_transf.pose_lerp(nextFrame.pose[id], progress));
            }

            rig.apply_pose(currentPose, math::mat4(1.f));
        }
    }

    void AnimationSystem::shutdown()
    {

    }
}
