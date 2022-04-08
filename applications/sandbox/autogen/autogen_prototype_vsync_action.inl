#include "autogen_prototype_vsync_action.hpp"
#include "../../sandbox/systems/simplecameracontroller.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::vsync_action>(const ::vsync_action& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::vsync_action>();
        prot.typeName = "vsync_action";
        return prot;
    }
}
