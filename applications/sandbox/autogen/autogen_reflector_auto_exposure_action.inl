#include "autogen_reflector_auto_exposure_action.hpp"
#include "../../sandbox/systems/examplesystem.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::auto_exposure_action>(::auto_exposure_action& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::auto_exposure_action>();
        refl.typeName = "auto_exposure_action";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::auto_exposure_action>(const ::auto_exposure_action& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::auto_exposure_action>();
        refl.typeName = "auto_exposure_action";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
