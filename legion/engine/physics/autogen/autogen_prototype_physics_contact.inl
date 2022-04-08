#include "autogen_prototype_physics_contact.hpp"
#include "../../physics/diviner/physics_contact.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::physics::physics_contact>(const legion::physics::physics_contact& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::physics::physics_contact>();
        prot.typeName = "legion::physics::physics_contact";
        return prot;
    }
}
