#include "autogen_prototype_archetype_base.hpp"
#include "../../core/ecs/archetype/archetype.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::ecs::archetype_base>(const legion::core::ecs::archetype_base& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::ecs::archetype_base>();
        prot.typeName = "legion::core::ecs::archetype_base";
        return prot;
    }
}
