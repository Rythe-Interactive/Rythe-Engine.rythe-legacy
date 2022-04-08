#include "autogen_reflector_exit_action.hpp"
#include "../../sandbox/systems/simplecameracontroller.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::exit_action>(::exit_action& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::exit_action>();
        refl.typeName = "exit_action";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::exit_action>(const ::exit_action& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::exit_action>();
        refl.typeName = "exit_action";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
