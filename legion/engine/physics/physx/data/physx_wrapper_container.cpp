#include "physx_wrapper_container.hpp"

namespace legion::physics
{
    PhysxInternalWrapper& PhysxWrapperContainer::createPhysxWrapper(physics_component& unregisteredPhysXWrapper)
    {
        unregisteredPhysXWrapper.physicsComponentID = nextID;
        m_wrapperIDSet.insert(nextID);

        m_physxWrappers.emplace_back();

        nextID++;
        return m_physxWrappers[m_physxWrappers.size() - 1];
    }

    void PhysxWrapperContainer::PopAndSwapRemoveWrapper(size_type id)
    {
        size_type index = m_wrapperIDSet.index_of(id);

        if (index != m_wrapperIDSet.npos)
        {
            m_wrapperIDSet.erase(id);
            std::swap(m_physxWrappers[index], m_physxWrappers[m_physxWrappers.size() - 1]);
            m_physxWrappers.pop_back();
        }
    }

    std::optional<std::reference_wrapper< PhysxInternalWrapper>> PhysxWrapperContainer::findWrapperWithID(size_type id)
    {
        if (m_wrapperIDSet.contains(id))
        {
            PhysxInternalWrapper& wrapper = m_physxWrappers[m_wrapperIDSet.index_of(id)];
            return std::optional<std::reference_wrapper< PhysxInternalWrapper>>{ wrapper };
        }

        return std::nullopt;
    }
}
