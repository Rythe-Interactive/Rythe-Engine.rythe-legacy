#include "autogen_reflector_fullscreen_action.hpp"
#include "../../sandbox/systems/simplecameracontroller.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::fullscreen_action>(::fullscreen_action& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::fullscreen_action>();
        refl.typeName = "fullscreen_action";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::fullscreen_action>(const ::fullscreen_action& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::fullscreen_action>();
        refl.typeName = "fullscreen_action";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
