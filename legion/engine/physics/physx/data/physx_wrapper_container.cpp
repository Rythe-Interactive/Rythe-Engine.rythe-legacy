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

    std::optional<std::reference_wrapper< PhysxInternalWrapper>> PhysxWrapperContainer::findWrapperWithID(size_type id)
    {
        if (m_wrapperIDSet.contains(id))
        {
            PhysxInternalWrapper& wrapper = m_physxWrappers[m_wrapperIDSet[id]];
            return std::optional<std::reference_wrapper< PhysxInternalWrapper>>{ wrapper };
        }

        return std::nullopt;
    }
}
