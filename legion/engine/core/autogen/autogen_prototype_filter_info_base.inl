#include "autogen_prototype_filter_info_base.hpp"
#include "../../core/ecs/filters/filter_info.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::ecs::filter_info_base>(const legion::core::ecs::filter_info_base& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::ecs::filter_info_base>();
        prot.typeName = "legion::core::ecs::filter_info_base";
        return prot;
    }
}
