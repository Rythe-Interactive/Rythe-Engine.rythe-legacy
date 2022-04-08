#include "autogen_reflector_component_base.hpp"
#include "../../core/ecs/handles/component.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::ecs::component_base>(legion::core::ecs::component_base& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::ecs::component_base>();
        refl.typeName = "legion::core::ecs::component_base";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::ecs::component_base>(const legion::core::ecs::component_base& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::ecs::component_base>();
        refl.typeName = "legion::core::ecs::component_base";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
