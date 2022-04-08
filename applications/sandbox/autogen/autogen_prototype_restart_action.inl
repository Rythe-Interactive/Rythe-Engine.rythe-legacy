#include "autogen_prototype_restart_action.hpp"
#include "../../sandbox/systems/simplecameracontroller.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::restart_action>(const ::restart_action& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::restart_action>();
        prot.typeName = "restart_action";
        return prot;
    }
}
