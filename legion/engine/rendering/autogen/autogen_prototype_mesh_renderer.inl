#include "autogen_prototype_mesh_renderer.hpp"
#include "../../rendering/components/renderable.hpp"
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::rendering::mesh_renderer>(const legion::rendering::mesh_renderer& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::rendering::mesh_renderer>();
        prot.typeName = "legion::rendering::mesh_renderer";
        {
            static const reflectable_attribute reflectable_attr{};
            prot.attributes.push_back(std::cref(reflectable_attr));
        }
        prot.members = std::vector<member_value>();
        {
            auto nested_prot = make_prototype(obj.material);
            prot.members.emplace_back("material",nested_prot);
        }
        return prot;
    }
}
