#pragma once
#include <core/core.hpp>
#include <physics/data/colliders/collider_data.hpp>

namespace legion::physics
{
    using physics_material_hash = size_type;

    constexpr physics_material_hash defaultMaterialHash = 0;

    class PhysicsHelpers
    {
    public:

        static void createPhysicsMaterial(float dynamicFriction, float staticFriction, float restitution, const char* name);

        static physics_material_hash retrievePhysicsMaterialHash(const char* name);

    private:

        PhysicsHelpers() = default;

        static std::vector<physics_material_hash> m_physicsMaterialHashes;

    };
}
