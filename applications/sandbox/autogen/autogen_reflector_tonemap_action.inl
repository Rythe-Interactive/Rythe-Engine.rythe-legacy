#include "autogen_reflector_tonemap_action.hpp"
#include "../../sandbox/systems/examplesystem.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::tonemap_action>(::tonemap_action& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::tonemap_action>();
        refl.typeName = "tonemap_action";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::tonemap_action>(const ::tonemap_action& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::tonemap_action>();
        refl.typeName = "tonemap_action";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
