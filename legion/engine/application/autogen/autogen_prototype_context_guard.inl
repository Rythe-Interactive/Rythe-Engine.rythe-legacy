#include "autogen_prototype_context_guard.hpp"
#include "../../application/window/window.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::application::context_guard>(const legion::application::context_guard& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::application::context_guard>();
        prot.typeName = "legion::application::context_guard";
        return prot;
    }
}
