
#include <physics/diviner/components/dvr_internal_physics_component.hpp>
#include <physics/diviner/colliders/convexcollider.hpp>
#include <physics/diviner/physics_statics.hpp>

namespace legion::physics
{
    void dvr_internal_physics_component::calculateNewLocalCenterOfMass()
    {
        localCenterOfMass = math::vec3::zero;

        for (auto collider : colliders)
        {
            localCenterOfMass += collider->GetLocalCentroid();
        }

        localCenterOfMass /= static_cast<float>(colliders.size());
    }

    std::shared_ptr<ConvexCollider> dvr_internal_physics_component::constructConvexHullFromVertices(const std::vector<math::vec3>& vertices)
    {
        auto collider = PhysicsStatics::generateConvexHull(vertices);

        if (collider)
        {
            colliders.push_back(collider);
            calculateNewLocalCenterOfMass();
        }

        return collider;
    }

    void dvr_internal_physics_component::ConstructBox()
    {

        calculateNewLocalCenterOfMass();
    }

    void dvr_internal_physics_component::AddBox(const cube_collider_params& cubeParams)
    {
        auto cuboidCollider = std::make_shared<ConvexCollider>();

        cuboidCollider->CreateBox(cubeParams);

        colliders.push_back(cuboidCollider);

        calculateNewLocalCenterOfMass();
    }

    void dvr_internal_physics_component::AddSphere()
    {
        calculateNewLocalCenterOfMass();
    }
}

