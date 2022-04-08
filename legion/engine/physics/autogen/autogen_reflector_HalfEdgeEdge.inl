#include "autogen_reflector_HalfEdgeEdge.hpp"
#include "../../physics/diviner/halfedgeedge.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::physics::HalfEdgeEdge>(legion::physics::HalfEdgeEdge& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::physics::HalfEdgeEdge>();
        refl.typeName = "legion::physics::HalfEdgeEdge";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::physics::HalfEdgeEdge>(const legion::physics::HalfEdgeEdge& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::physics::HalfEdgeEdge>();
        refl.typeName = "legion::physics::HalfEdgeEdge";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
