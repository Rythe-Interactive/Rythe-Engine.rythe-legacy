#include "autogen_prototype_collision_event.hpp"
#include "../../physics/diviner/events/events.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::physics::collision_event>(const legion::physics::collision_event& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::physics::collision_event>();
        prot.typeName = "legion::physics::collision_event";
        return prot;
    }
}
