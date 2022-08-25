#include "physx_wrapper_container.hpp"
#include <physics/components/physics_component.hpp>
#include <physics/components/physics_enviroment.hpp>
#include <physics/components/capsule_controller.hpp>
#include <physics/physx/data/physx_wrapper.hpp>

namespace legion::physics
{
    template<class PhysxObject>
    PhysxObject& PhysxWrapperContainer<PhysxObject>::createPhysxWrapper(physics_component& unregisteredPhysXWrapper)
    {
        return registerWrapperID(unregisteredPhysXWrapper.physicsComponentID);
    }

    template<class PhysxObject>
    PhysxObject& PhysxWrapperContainer<PhysxObject>::createPhysxWrapper(physics_enviroment& unregisteredPhysXWrapper)
    {
        return registerWrapperID(unregisteredPhysXWrapper.physicsEnviromentID);
    }

    template<class PhysxObject>
    PhysxObject& PhysxWrapperContainer<PhysxObject>::createPhysxWrapper(capsule_controller& unregisteredPhysXWrapper)
    {
        return registerWrapperID(unregisteredPhysXWrapper.id);
    }

    template<class PhysxObject>
    void PhysxWrapperContainer<PhysxObject>::PopAndSwapRemoveWrapper(size_type id)
    {
        size_type index = m_wrapperIDSet.index_of(id);

        if (index != m_wrapperIDSet.npos)
        {
            m_wrapperIDSet.erase(id);
            std::swap(m_physxWrappers[index], m_physxWrappers[m_physxWrappers.size() - 1]);
            m_physxWrappers.pop_back();
        }
    }

    template<class PhysxObject>
    pointer<PhysxObject> PhysxWrapperContainer<PhysxObject>::findWrapperWithID(size_type id)
    {
        if (m_wrapperIDSet.contains(id))
        {
            return pointer< PhysxObject>{ &m_physxWrappers[m_wrapperIDSet.index_of(id)] };
        }

        return { nullptr };
    }

    template<class PhysxObject>
    PhysxObject& PhysxWrapperContainer<PhysxObject>::registerWrapperID(size_type& outID)
    {
        outID = nextID;
        m_wrapperIDSet.insert(nextID);

        m_physxWrappers.emplace_back();

        nextID++;
        return m_physxWrappers[m_physxWrappers.size() - 1];
    }

    template class PhysxWrapperContainer<PhysxInternalWrapper>;
    template class PhysxWrapperContainer<PhysxCharacterWrapper>;
}
