#include "autogen_prototype_mesh_filter.hpp"
#include "../../core/defaults/defaultcomponents.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::mesh_filter>(const legion::core::mesh_filter& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::mesh_filter>();
        prot.typeName = "legion::core::mesh_filter";
        {
            static const ::reflectable_attribute reflectable_attr{};
            prot.attributes.push_back(std::cref(reflectable_attr));
        }
        prot.members = std::vector<member_value>();
        {
            auto nested_prot = make_prototype(obj.shared_mesh);
            prot.members.emplace_back("shared_mesh",nested_prot);
        }
        return prot;
    }
}
