#include "autogen_reflector_entity.hpp"
#include "../../core/ecs/handles/entity.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::ecs::entity>(legion::core::ecs::entity& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::ecs::entity>();
        refl.typeName = "legion::core::ecs::entity";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::ecs::entity>(const legion::core::ecs::entity& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::ecs::entity>();
        refl.typeName = "legion::core::ecs::entity";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
