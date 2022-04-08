#include "autogen_reflector_SpawnHullActive.hpp"
#include "../../sandbox/systems/physics_test_system.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::SpawnHullActive>(::SpawnHullActive& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::SpawnHullActive>();
        refl.typeName = "SpawnHullActive";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::SpawnHullActive>(const ::SpawnHullActive& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::SpawnHullActive>();
        refl.typeName = "SpawnHullActive";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
