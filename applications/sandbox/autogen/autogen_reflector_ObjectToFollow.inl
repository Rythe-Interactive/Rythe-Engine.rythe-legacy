#include "autogen_reflector_ObjectToFollow.hpp"
#include "../../sandbox/systems/physics_test_system.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<::ObjectToFollow>(::ObjectToFollow& obj)
    {
        reflector refl;
        refl.typeId = typeHash<::ObjectToFollow>();
        refl.typeName = "ObjectToFollow";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const ::ObjectToFollow>(const ::ObjectToFollow& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<::ObjectToFollow>();
        refl.typeName = "ObjectToFollow";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
