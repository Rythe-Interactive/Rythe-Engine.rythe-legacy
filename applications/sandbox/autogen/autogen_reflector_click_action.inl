#include "autogen_reflector_click_action.hpp"
#include "../../sandbox/systems/gui_test.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::click_action>(legion::click_action& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::click_action>();
        refl.typeName = "legion::click_action";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::click_action>(const legion::click_action& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::click_action>();
        refl.typeName = "legion::click_action";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
