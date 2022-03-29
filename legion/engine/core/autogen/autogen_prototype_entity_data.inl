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
        {
            static const ::reflectable_attribute reflectable_attr{};
            prot.attributes.push_back(std::cref(reflectable_attr));
        }
        prot.members = std::vector<member_value>{
        member_value
        {
            "alive",
            primitive_value {typeHash<bool>(),std::make_any<bool>(obj.alive)}
        }
        ,
        member_value
        {
            "active",
            primitive_value {typeHash<bool>(),std::make_any<bool>(obj.active)}
        }
        };
        {
            auto nested_prot = make_prototype(obj.parent);
            prot.members.emplace_back("parent",nested_prot);
        }
        return prot;
    }
}
