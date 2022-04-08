#include "autogen_prototype_trigger_event.hpp"
#include "../../physics/diviner/events/events.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::physics::trigger_event>(const legion::physics::trigger_event& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::physics::trigger_event>();
        prot.typeName = "legion::physics::trigger_event";
        return prot;
    }
}
