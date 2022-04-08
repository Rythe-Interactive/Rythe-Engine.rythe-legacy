#include "autogen_prototype_HalfEdgeEdge.hpp"
#include "../../physics/diviner/halfedgeedge.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::physics::HalfEdgeEdge>(const legion::physics::HalfEdgeEdge& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::physics::HalfEdgeEdge>();
        prot.typeName = "legion::physics::HalfEdgeEdge";
        return prot;
    }
}
