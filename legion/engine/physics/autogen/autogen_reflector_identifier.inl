#include "autogen_reflector_identifier.hpp"
#include "../../physics/diviner/data/identifier.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::physics::identifier>(legion::physics::identifier& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::physics::identifier>();
        refl.typeName = "legion::physics::identifier";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::physics::identifier>(const legion::physics::identifier& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::physics::identifier>();
        refl.typeName = "legion::physics::identifier";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
