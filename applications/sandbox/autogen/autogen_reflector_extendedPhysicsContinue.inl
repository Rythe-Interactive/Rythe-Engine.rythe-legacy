#include "autogen_reflector_extendedPhysicsContinue.hpp"
#include "../../sandbox/systems/physics_test_system.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::extendedPhysicsContinue>(::extendedPhysicsContinue& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::extendedPhysicsContinue>();
        refl.typeName = "extendedPhysicsContinue";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::extendedPhysicsContinue>(const ::extendedPhysicsContinue& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::extendedPhysicsContinue>();
        refl.typeName = "extendedPhysicsContinue";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
