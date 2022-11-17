#pragma once
#include <core/core.hpp>

namespace legion::physics
{
    /** @struct FractureParams
    * @brief contains the parameters that will be used for a given fracture event.
    */
    struct FractureParams
    {

        math::float3 explosionCentroid;
        float strength;

        FractureParams(math::float3 pWorldExplostionCent, float pStrength = 5.0f)
            : explosionCentroid(pWorldExplostionCent),strength(pStrength)
        {

        }
    };
}

