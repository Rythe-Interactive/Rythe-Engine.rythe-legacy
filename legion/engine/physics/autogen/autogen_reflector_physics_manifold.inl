#include "autogen_reflector_physics_manifold.hpp"
#include "../../physics/diviner/data/physics_manifold.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::physics::physics_manifold>(legion::physics::physics_manifold& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::physics::physics_manifold>();
        refl.typeName = "legion::physics::physics_manifold";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::physics::physics_manifold>(const legion::physics::physics_manifold& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::physics::physics_manifold>();
        refl.typeName = "legion::physics::physics_manifold";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
