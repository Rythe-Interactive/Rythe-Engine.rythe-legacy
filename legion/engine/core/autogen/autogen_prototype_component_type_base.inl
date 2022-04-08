#include "autogen_prototype_component_type_base.hpp"
#include "../../core/ecs/data/component_type.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::ecs::component_type_base>(const legion::core::ecs::component_type_base& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::ecs::component_type_base>();
        prot.typeName = "legion::core::ecs::component_type_base";
        return prot;
    }
}
