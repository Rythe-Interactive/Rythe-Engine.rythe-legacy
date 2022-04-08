#include "autogen_reflector_physics_contact.hpp"
#include "../../physics/diviner/physics_contact.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::physics::physics_contact>(legion::physics::physics_contact& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::physics::physics_contact>();
        refl.typeName = "legion::physics::physics_contact";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::physics::physics_contact>(const legion::physics::physics_contact& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::physics::physics_contact>();
        refl.typeName = "legion::physics::physics_contact";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
