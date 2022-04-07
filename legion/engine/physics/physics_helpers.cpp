#include "physics_helpers.hpp"
#include <physics/events/events.hpp>
namespace legion::physics
{
    delegate<bool(size_type, const char*)> PhysicsHelpers::m_debugMaterialRetrieveCheck =
        [](size_type hash, const char* materialName)->bool {
        log::warn("m_debugMaterialRetrieveCheck not set, make sure the currently active physics engine sets this delegate!");
        return true;
    };

    void PhysicsHelpers::createPhysicsMaterial(float dynamicFriction, float staticFriction, float restitution, const char* name)
    {
        size_type hashResult = nameHash(name);
        events::EventBus::raiseEvent< request_create_physics_material>(request_create_physics_material{ dynamicFriction,staticFriction,restitution,hashResult});
    }

    size_type PhysicsHelpers::retrievePhysicsMaterialHash(const char* name)
    {
        size_type hashResult = nameHash(name);

        #ifdef  _DEBUG
        m_debugMaterialRetrieveCheck.invoke(hashResult, name);
        #endif 

        return hashResult;
    }
}
