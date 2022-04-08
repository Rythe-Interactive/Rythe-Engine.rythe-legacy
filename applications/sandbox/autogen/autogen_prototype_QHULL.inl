#include "autogen_prototype_QHULL.hpp"
#include "../../sandbox/systems/physics_test_system.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::QHULL>(const ::QHULL& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::QHULL>();
        prot.typeName = "QHULL";
        return prot;
    }
}
