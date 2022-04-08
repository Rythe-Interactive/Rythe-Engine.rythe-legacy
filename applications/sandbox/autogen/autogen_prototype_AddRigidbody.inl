#include "autogen_prototype_AddRigidbody.hpp"
#include "../../sandbox/systems/physics_test_system.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::AddRigidbody>(const ::AddRigidbody& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::AddRigidbody>();
        prot.typeName = "AddRigidbody";
        return prot;
    }
}
