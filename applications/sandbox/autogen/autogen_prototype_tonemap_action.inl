#include "autogen_prototype_tonemap_action.hpp"
#include "../../sandbox/systems/examplesystem.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::tonemap_action>(const ::tonemap_action& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::tonemap_action>();
        prot.typeName = "tonemap_action";
        return prot;
    }
}
