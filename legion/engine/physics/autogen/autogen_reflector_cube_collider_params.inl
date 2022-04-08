#include "autogen_reflector_cube_collider_params.hpp"
#include "../../physics/diviner/cube_collider_params.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::physics::cube_collider_params>(legion::physics::cube_collider_params& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::physics::cube_collider_params>();
        refl.typeName = "legion::physics::cube_collider_params";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::physics::cube_collider_params>(const legion::physics::cube_collider_params& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::physics::cube_collider_params>();
        refl.typeName = "legion::physics::cube_collider_params";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
