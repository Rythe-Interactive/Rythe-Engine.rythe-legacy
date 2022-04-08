#include "autogen_reflector_switch_skybox_action.hpp"
#include "../../sandbox/systems/examplesystem.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::switch_skybox_action>(::switch_skybox_action& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::switch_skybox_action>();
        refl.typeName = "switch_skybox_action";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::switch_skybox_action>(const ::switch_skybox_action& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::switch_skybox_action>();
        refl.typeName = "switch_skybox_action";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
