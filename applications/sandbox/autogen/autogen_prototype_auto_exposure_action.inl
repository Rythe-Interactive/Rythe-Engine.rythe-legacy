#include "autogen_prototype_auto_exposure_action.hpp"
#include "../../sandbox/systems/examplesystem.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::auto_exposure_action>(const ::auto_exposure_action& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::auto_exposure_action>();
        prot.typeName = "auto_exposure_action";
        return prot;
    }
}
