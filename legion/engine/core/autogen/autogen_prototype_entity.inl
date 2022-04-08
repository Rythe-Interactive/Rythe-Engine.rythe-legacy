#include "autogen_prototype_entity.hpp"
#include "../../core/ecs/handles/entity.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::ecs::entity>(const legion::core::ecs::entity& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::ecs::entity>();
        prot.typeName = "legion::core::ecs::entity";
        return prot;
    }
}
