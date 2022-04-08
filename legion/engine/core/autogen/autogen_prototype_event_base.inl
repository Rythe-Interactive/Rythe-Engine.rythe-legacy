#include "autogen_prototype_event_base.hpp"
#include "../../core/events/event.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::events::event_base>(const legion::core::events::event_base& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::events::event_base>();
        prot.typeName = "legion::core::events::event_base";
        return prot;
    }
}
