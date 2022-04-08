#include "autogen_prototype_component_base.hpp"
#include "../../core/ecs/handles/component.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::ecs::component_base>(const legion::core::ecs::component_base& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::ecs::component_base>();
        prot.typeName = "legion::core::ecs::component_base";
        return prot;
    }
}
