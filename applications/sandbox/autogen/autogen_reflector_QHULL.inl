#include "autogen_reflector_QHULL.hpp"
#include "../../sandbox/systems/physics_test_system.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::QHULL>(::QHULL& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::QHULL>();
        refl.typeName = "QHULL";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::QHULL>(const ::QHULL& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::QHULL>();
        refl.typeName = "QHULL";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
