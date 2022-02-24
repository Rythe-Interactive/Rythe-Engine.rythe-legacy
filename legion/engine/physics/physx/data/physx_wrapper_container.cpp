#include "physx_wrapper_container.hpp"

namespace legion::physics
{
    PhysxInternalWrapper& PhysxWrapperContainer::createPhysxWrapper(physicsComponent& unregisteredPhysXWrapper)
    {
        unregisteredPhysXWrapper.physicsComponentID = nextID;
        m_wrapperIDSet.insert(nextID);
        m_physxWrappers.emplace_back(PhysxInternalWrapper());

        nextID++;
        return m_physxWrappers[m_physxWrappers.size() - 1];
    }
}
