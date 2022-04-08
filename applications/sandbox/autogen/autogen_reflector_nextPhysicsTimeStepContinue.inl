#include "autogen_reflector_nextPhysicsTimeStepContinue.hpp"
#include "../../sandbox/systems/physics_test_system.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::nextPhysicsTimeStepContinue>(::nextPhysicsTimeStepContinue& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::nextPhysicsTimeStepContinue>();
        refl.typeName = "nextPhysicsTimeStepContinue";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::nextPhysicsTimeStepContinue>(const ::nextPhysicsTimeStepContinue& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::nextPhysicsTimeStepContinue>();
        refl.typeName = "nextPhysicsTimeStepContinue";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
