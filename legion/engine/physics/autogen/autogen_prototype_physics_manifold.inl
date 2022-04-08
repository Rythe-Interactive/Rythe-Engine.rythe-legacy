#include "autogen_prototype_physics_manifold.hpp"
#include "../../physics/diviner/data/physics_manifold.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::physics::physics_manifold>(const legion::physics::physics_manifold& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::physics::physics_manifold>();
        prot.typeName = "legion::physics::physics_manifold";
        return prot;
    }
}
