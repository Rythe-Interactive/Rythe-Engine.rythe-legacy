#include "autogen_prototype_nextPhysicsTimeStepContinue.hpp"
#include "../../sandbox/systems/physics_test_system.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::nextPhysicsTimeStepContinue>(const ::nextPhysicsTimeStepContinue& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::nextPhysicsTimeStepContinue>();
        prot.typeName = "nextPhysicsTimeStepContinue";
        return prot;
    }
}
