#include "autogen_prototype_physicsComponent.hpp"
#include "../../physics/diviner/components/physics_component.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::physics::physicsComponent>(const legion::physics::physicsComponent& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::physics::physicsComponent>();
        prot.typeName = "legion::physics::physicsComponent";
        return prot;
    }
}
