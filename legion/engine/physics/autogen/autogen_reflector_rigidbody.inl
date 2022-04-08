#include "autogen_reflector_rigidbody.hpp"
#include "../../physics/diviner/components/rigidbody.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::physics::rigidbody>(legion::physics::rigidbody& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::physics::rigidbody>();
        refl.typeName = "legion::physics::rigidbody";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::physics::rigidbody>(const legion::physics::rigidbody& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::physics::rigidbody>();
        refl.typeName = "legion::physics::rigidbody";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
