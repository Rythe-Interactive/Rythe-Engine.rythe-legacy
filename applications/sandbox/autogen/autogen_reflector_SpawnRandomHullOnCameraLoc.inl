#include "autogen_reflector_SpawnRandomHullOnCameraLoc.hpp"
#include "../../sandbox/systems/physics_test_system.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::SpawnRandomHullOnCameraLoc>(::SpawnRandomHullOnCameraLoc& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::SpawnRandomHullOnCameraLoc>();
        refl.typeName = "SpawnRandomHullOnCameraLoc";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::SpawnRandomHullOnCameraLoc>(const ::SpawnRandomHullOnCameraLoc& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::SpawnRandomHullOnCameraLoc>();
        refl.typeName = "SpawnRandomHullOnCameraLoc";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
