#include "autogen_prototype_reload_shaders_action.hpp"
#include "../../sandbox/systems/examplesystem.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::reload_shaders_action>(const ::reload_shaders_action& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::reload_shaders_action>();
        prot.typeName = "reload_shaders_action";
        return prot;
    }
}
