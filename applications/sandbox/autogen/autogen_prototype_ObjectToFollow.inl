#include "autogen_prototype_ObjectToFollow.hpp"
#include "../../sandbox/systems/physics_test_system.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<::ObjectToFollow>(const ::ObjectToFollow& obj)
    {
        prototype prot;
        prot.typeId = typeHash<::ObjectToFollow>();
        prot.typeName = "ObjectToFollow";
        return prot;
    }
}
