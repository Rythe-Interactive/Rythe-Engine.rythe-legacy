#include "autogen_prototype_rigidbody.hpp"
#include "../../physics/diviner/components/rigidbody.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::physics::rigidbody>(const legion::physics::rigidbody& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::physics::rigidbody>();
        prot.typeName = "legion::physics::rigidbody";
        return prot;
    }
}
