#include "autogen_prototype_switch_skybox_action.hpp"
#include "../../sandbox/systems/examplesystem.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::switch_skybox_action>(const ::switch_skybox_action& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::switch_skybox_action>();
        prot.typeName = "switch_skybox_action";
        return prot;
    }
}
