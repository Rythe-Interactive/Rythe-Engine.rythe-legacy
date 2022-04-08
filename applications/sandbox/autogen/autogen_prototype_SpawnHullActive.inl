#include "autogen_prototype_SpawnHullActive.hpp"
#include "../../sandbox/systems/physics_test_system.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::SpawnHullActive>(const ::SpawnHullActive& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::SpawnHullActive>();
        prot.typeName = "SpawnHullActive";
        return prot;
    }
}
