#include "autogen_prototype_physics_manifold_precursor.hpp"
#include "../../physics/diviner/data/physics_manifold_precursor.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::physics::physics_manifold_precursor>(const legion::physics::physics_manifold_precursor& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::physics::physics_manifold_precursor>();
        prot.typeName = "legion::physics::physics_manifold_precursor";
        return prot;
    }
}
