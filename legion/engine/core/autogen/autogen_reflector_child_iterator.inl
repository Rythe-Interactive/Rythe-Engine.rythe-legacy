#include "autogen_reflector_child_iterator.hpp"
#include "../../core/ecs/handles/entity.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::ecs::child_iterator>(legion::core::ecs::child_iterator& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::ecs::child_iterator>();
        refl.typeName = "legion::core::ecs::child_iterator";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::ecs::child_iterator>(const legion::core::ecs::child_iterator& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::ecs::child_iterator>();
        refl.typeName = "legion::core::ecs::child_iterator";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
