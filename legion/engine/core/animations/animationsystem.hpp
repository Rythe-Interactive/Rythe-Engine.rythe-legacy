#pragma once
#include <core/data/joint.hpp>
#include <core/data/animation.hpp>
#include <core/engine/system.inl>
#include <core/defaults/defaultcomponents.hpp>

namespace legion::core
{
    class AnimationSystem final : public System<AnimationSystem>
    {
        ecs::filter<mesh_filter, animator> filter;
    public:
        void setup();
        void update(time::span deltaTime);
        void apply_pose(joint& parentJoint, std::unordered_map<size_type, math::mat4> currentPose, math::mat4 parentTransf);
        void shutdown();
    };
}
