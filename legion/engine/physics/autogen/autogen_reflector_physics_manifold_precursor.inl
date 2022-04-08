#include "autogen_reflector_physics_manifold_precursor.hpp"
#include "../../physics/diviner/data/physics_manifold_precursor.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::physics::physics_manifold_precursor>(legion::physics::physics_manifold_precursor& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::physics::physics_manifold_precursor>();
        refl.typeName = "legion::physics::physics_manifold_precursor";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::physics::physics_manifold_precursor>(const legion::physics::physics_manifold_precursor& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::physics::physics_manifold_precursor>();
        refl.typeName = "legion::physics::physics_manifold_precursor";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
