#include "autogen_reflector_physicsComponent.hpp"
#include "../../physics/diviner/components/physics_component.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::physics::physicsComponent>(legion::physics::physicsComponent& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::physics::physicsComponent>();
        refl.typeName = "legion::physics::physicsComponent";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::physics::physicsComponent>(const legion::physics::physicsComponent& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::physics::physicsComponent>();
        refl.typeName = "legion::physics::physicsComponent";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
