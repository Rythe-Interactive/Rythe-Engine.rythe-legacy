#include "autogen_reflector_reload_shaders_action.hpp"
#include "../../sandbox/systems/examplesystem.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::reload_shaders_action>(::reload_shaders_action& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::reload_shaders_action>();
        refl.typeName = "reload_shaders_action";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::reload_shaders_action>(const ::reload_shaders_action& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::reload_shaders_action>();
        refl.typeName = "reload_shaders_action";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
