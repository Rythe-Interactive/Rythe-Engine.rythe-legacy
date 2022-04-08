#include "autogen_reflector_archetype_base.hpp"
#include "../../core/ecs/archetype/archetype.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::ecs::archetype_base>(legion::core::ecs::archetype_base& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::ecs::archetype_base>();
        refl.typeName = "legion::core::ecs::archetype_base";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::ecs::archetype_base>(const legion::core::ecs::archetype_base& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::ecs::archetype_base>();
        refl.typeName = "legion::core::ecs::archetype_base";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
