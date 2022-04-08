#include "autogen_prototype_identifier.hpp"
#include "../../physics/diviner/data/identifier.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::physics::identifier>(const legion::physics::identifier& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::physics::identifier>();
        prot.typeName = "legion::physics::identifier";
        return prot;
    }
}
