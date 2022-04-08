#include "autogen_prototype_fullscreen_action.hpp"
#include "../../sandbox/systems/simplecameracontroller.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::fullscreen_action>(const ::fullscreen_action& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::fullscreen_action>();
        prot.typeName = "fullscreen_action";
        return prot;
    }
}
