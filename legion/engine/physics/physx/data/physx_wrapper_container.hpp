#pragma once
#include <physics/physx/data/physx_wrapper.hpp>
#include <physics/components/physics_component.hpp>
#include <physics/components/physics_enviroment.hpp>

namespace legion::physics
{
    class PhysxWrapperContainer
    {
    public:

        PhysxInternalWrapper& createPhysxWrapper(physics_component& unregisteredPhysXWrapper);

        PhysxInternalWrapper& createPhysxWrapper(physics_enviroment& unregisteredPhysXWrapper);

        void PopAndSwapRemoveWrapper(size_type id);

        pointer<PhysxInternalWrapper> findWrapperWithID(size_type id);

        inline void ReleasePhysicsWrappers() { m_physxWrappers.clear(); }

    private:

        PhysxInternalWrapper& registerWrapperID(size_type& outID);

        sparse_set<size_type> m_wrapperIDSet;
        std::vector<PhysxInternalWrapper> m_physxWrappers;

        size_type nextID = 0;
    };

}

