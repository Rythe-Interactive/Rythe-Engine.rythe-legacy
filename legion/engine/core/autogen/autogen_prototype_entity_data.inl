#include "autogen_prototype_entity_data.hpp"
#include "../../core/ecs/data/entity_data.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::ecs::entity_data>(const legion::core::ecs::entity_data& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::ecs::entity_data>();
        prot.typeName = "legion::core::ecs::entity_data";
        return prot;
    }
}
