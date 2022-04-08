#include "autogen_reflector_collision_event.hpp"
#include "../../physics/diviner/events/events.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::physics::collision_event>(legion::physics::collision_event& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::physics::collision_event>();
        refl.typeName = "legion::physics::collision_event";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::physics::collision_event>(const legion::physics::collision_event& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::physics::collision_event>();
        refl.typeName = "legion::physics::collision_event";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
