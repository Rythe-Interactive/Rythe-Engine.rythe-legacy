#include "autogen_reflector_const_child_reverse_iterator.hpp"
#include "../../core/ecs/handles/entity.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::ecs::const_child_reverse_iterator>(legion::core::ecs::const_child_reverse_iterator& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::ecs::const_child_reverse_iterator>();
        refl.typeName = "legion::core::ecs::const_child_reverse_iterator";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::ecs::const_child_reverse_iterator>(const legion::core::ecs::const_child_reverse_iterator& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::ecs::const_child_reverse_iterator>();
        refl.typeName = "legion::core::ecs::const_child_reverse_iterator";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
