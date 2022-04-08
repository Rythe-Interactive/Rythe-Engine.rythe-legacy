#include "autogen_reflector_AddRigidbody.hpp"
#include "../../sandbox/systems/physics_test_system.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::AddRigidbody>(::AddRigidbody& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::AddRigidbody>();
        refl.typeName = "AddRigidbody";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::AddRigidbody>(const ::AddRigidbody& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::AddRigidbody>();
        refl.typeName = "AddRigidbody";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
