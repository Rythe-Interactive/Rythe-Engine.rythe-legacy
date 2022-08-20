#pragma once
#include <core/types/types.hpp>
#include <core/math/math.hpp>

namespace legion::core
{
    struct joint
    {
        id_type id;
        std::string name;
        std::vector<joint> children;
        math::mat4 animatedTransform;//represents a joints transformation from old to new position

       /* math::mat4 localBindTransform;*///represents a joints transformation from its parent
        math::mat4 invBindTransform;//represents the transformation from the joint to the root

        joint() = default;
        joint(id_type id, math::mat4 invBindTransform) : id(id), invBindTransform(invBindTransform) {}
        joint(std::string name, id_type id, math::mat4 invBindTransform) : id(id), name(name), invBindTransform(invBindTransform) {}
        ~joint() = default;

        //inline void calc_inv_bind_transform(math::mat4 parentBindTransform)
        //{
        //    math::mat4 bindTransform = parentBindTransform * localBindTransform;
        //    invBindTransform = math::inverse(bindTransform);
        //    for (joint child : children)
        //        child.calc_inv_bind_transform(bindTransform);
        //}
    };
}
