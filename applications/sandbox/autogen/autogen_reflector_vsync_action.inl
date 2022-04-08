#include "autogen_reflector_vsync_action.hpp"
#include "../../sandbox/systems/simplecameracontroller.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::vsync_action>(::vsync_action& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::vsync_action>();
        refl.typeName = "vsync_action";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::vsync_action>(const ::vsync_action& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::vsync_action>();
        refl.typeName = "vsync_action";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
