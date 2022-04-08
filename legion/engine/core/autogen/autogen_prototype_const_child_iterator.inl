#include "autogen_prototype_const_child_iterator.hpp"
#include "../../core/ecs/handles/entity.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::ecs::const_child_iterator>(const legion::core::ecs::const_child_iterator& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::ecs::const_child_iterator>();
        prot.typeName = "legion::core::ecs::const_child_iterator";
        return prot;
    }
}
