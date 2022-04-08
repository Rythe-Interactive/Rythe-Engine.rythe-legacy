#include "autogen_reflector_restart_action.hpp"
#include "../../sandbox/systems/simplecameracontroller.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::restart_action>(::restart_action& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::restart_action>();
        refl.typeName = "restart_action";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::restart_action>(const ::restart_action& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::restart_action>();
        refl.typeName = "restart_action";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
