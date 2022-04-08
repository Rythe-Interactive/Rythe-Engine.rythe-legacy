#include "autogen_reflector_ContactVertex.hpp"
#include "../../physics/diviner/data/contact_vertex.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::physics::ContactVertex>(legion::physics::ContactVertex& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::physics::ContactVertex>();
        refl.typeName = "legion::physics::ContactVertex";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::physics::ContactVertex>(const legion::physics::ContactVertex& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::physics::ContactVertex>();
        refl.typeName = "legion::physics::ContactVertex";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
