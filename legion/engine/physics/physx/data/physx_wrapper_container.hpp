#pragma once
#include <physics/physx/data/physx_wrapper.hpp>
#include <physics/components/physics_component.hpp>

namespace legion::physics
{
    class PhysxWrapperContainer
    {
    public:

        PhysxInternalWrapper& createPhysxWrapper(physicsComponent& unregisteredPhysXWrapper);

        void PopAndSwapRemoveWrapper(size_type id);

        std::optional<std::reference_wrapper< PhysxInternalWrapper>> findWrapperWithID(size_type id);

    private:
        sparse_set<size_type> m_wrapperIDSet;
        std::vector<PhysxInternalWrapper> m_physxWrappers;

        size_type nextID = 0;
    };

}

