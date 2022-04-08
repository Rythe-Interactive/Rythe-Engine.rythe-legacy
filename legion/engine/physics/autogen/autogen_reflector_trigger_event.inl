#include "autogen_reflector_trigger_event.hpp"
#include "../../physics/diviner/events/events.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::physics::trigger_event>(legion::physics::trigger_event& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::physics::trigger_event>();
        refl.typeName = "legion::physics::trigger_event";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::physics::trigger_event>(const legion::physics::trigger_event& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::physics::trigger_event>();
        refl.typeName = "legion::physics::trigger_event";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
