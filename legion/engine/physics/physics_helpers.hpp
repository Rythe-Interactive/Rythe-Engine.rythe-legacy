#pragma once
#include <core/core.hpp>
#include <physics/data/colliders/collider_data.hpp>

namespace legion::physics
{
    class PhysicsHelpers
    {
    public:

        static void createPhysicsMaterial(float dynamicFriction, float staticFriction, float restitution, const char* name);

        static size_type retrievePhysicsMaterialHash(const char* name);

        static void bindDebugRetrieveCheck(delegate<bool(size_type, const char*)> retrieveDelegate)
        {
            m_debugMaterialRetrieveCheck = retrieveDelegate;
        }

    private:
        static delegate<bool(size_type,const char*)> m_debugMaterialRetrieveCheck;
    };
}
