#include "autogen_reflector_entity_data.hpp"
#include "../../core/ecs/data/entity_data.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::ecs::entity_data>(legion::core::ecs::entity_data& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::ecs::entity_data>();
        refl.typeName = "legion::core::ecs::entity_data";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::ecs::entity_data>(const legion::core::ecs::entity_data& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::ecs::entity_data>();
        refl.typeName = "legion::core::ecs::entity_data";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
