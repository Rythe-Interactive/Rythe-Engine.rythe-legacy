#pragma once
#include <core/core.hpp>

namespace legion::physics
{
    struct gravity_preset
    {
        math::vec3 gravityValue = math::vec3(0.0f);
        math::vec3 gravityAcc = math::vec3(0.0f);
    };

    //using this preset must be set when the 
    struct rigidbody_force_feedback
    {
        float forceAmount = 0.0f;
        float massMaximum = 0.0f;//if massMaximum is <= 0, all rigidbodies will be pushed regardless of mass
    };



}
