#include "autogen_prototype_ContactVertex.hpp"
#include "../../physics/diviner/data/contact_vertex.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::physics::ContactVertex>(const legion::physics::ContactVertex& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::physics::ContactVertex>();
        prot.typeName = "legion::physics::ContactVertex";
        return prot;
    }
}
