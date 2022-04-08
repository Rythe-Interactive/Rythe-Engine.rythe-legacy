#include "autogen_prototype_exit_action.hpp"
#include "../../sandbox/systems/simplecameracontroller.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::exit_action>(const ::exit_action& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::exit_action>();
        prot.typeName = "exit_action";
        return prot;
    }
}
