#include "autogen_reflector_EdgeLabel.hpp"
#include "../../physics/diviner/data/edge_label.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::physics::EdgeLabel>(legion::physics::EdgeLabel& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::physics::EdgeLabel>();
        refl.typeName = "legion::physics::EdgeLabel";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::physics::EdgeLabel>(const legion::physics::EdgeLabel& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::physics::EdgeLabel>();
        refl.typeName = "legion::physics::EdgeLabel";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
