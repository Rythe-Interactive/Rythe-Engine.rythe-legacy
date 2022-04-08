#include "autogen_prototype_extendedPhysicsContinue.hpp"
#include "../../sandbox/systems/physics_test_system.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::extendedPhysicsContinue>(const ::extendedPhysicsContinue& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::extendedPhysicsContinue>();
        prot.typeName = "extendedPhysicsContinue";
        return prot;
    }
}
