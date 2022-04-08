#include "autogen_prototype_component_pool_base.hpp"
#include "../../core/ecs/containers/component_pool.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::ecs::component_pool_base>(const legion::core::ecs::component_pool_base& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::ecs::component_pool_base>();
        prot.typeName = "legion::core::ecs::component_pool_base";
        return prot;
    }
}
