#include "autogen_prototype_click_action.hpp"
#include "../../sandbox/systems/gui_test.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::click_action>(const legion::click_action& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::click_action>();
        prot.typeName = "legion::click_action";
        return prot;
    }
}
