#include "autogen_prototype_SpawnRandomHullOnCameraLoc.hpp"
#include "../../sandbox/systems/physics_test_system.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::SpawnRandomHullOnCameraLoc>(const ::SpawnRandomHullOnCameraLoc& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::SpawnRandomHullOnCameraLoc>();
        prot.typeName = "SpawnRandomHullOnCameraLoc";
        return prot;
    }
}
