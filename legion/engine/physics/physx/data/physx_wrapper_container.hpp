#pragma once
#include <physics/physx/data/physx_wrapper.hpp>
#include <physics/components/physics_component.hpp>

namespace legion::physics
{
    class PhysxWrapperContainer
    {
    public:

        PhysxInternalWrapper& createPhysxWrapper(physicsComponent& unregisteredPhysXWrapper);

    private:
        sparse_set<size_type> m_wrapperIDSet;
        std::vector<PhysxInternalWrapper> m_physxWrappers;

        size_type nextID = 0;
    };

}

