#include "physics_helpers.hpp"
#include <physics/events/events.hpp>
namespace legion::physics
{
    std::vector<physics_material_hash> PhysicsHelpers::m_physicsMaterialHashes;

    void PhysicsHelpers::createPhysicsMaterial(float dynamicFriction, float staticFriction, float restitution, const char* name)
    {
        size_type hashResult = nameHash(name);

        m_physicsMaterialHashes.push_back(hashResult);
        events::EventBus::raiseEvent<request_create_physics_material>(request_create_physics_material{ dynamicFriction,staticFriction,restitution,hashResult});
    }

    size_type PhysicsHelpers::retrievePhysicsMaterialHash(const char* name)
    {
        size_type hashResult = nameHash(name);

        for (physics_material_hash materialHash : m_physicsMaterialHashes)
        {
            if (hashResult == materialHash)
            {
                return hashResult;
            }
        }

        log::warn("Attempted to retrieve non existing physics material with name {0}, default material was returned instead ",name);
        return defaultMaterialHash;
    }
}
