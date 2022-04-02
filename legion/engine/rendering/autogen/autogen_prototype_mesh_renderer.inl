#include "autogen_prototype_mesh_renderer.hpp"
#include "../../rendering/components/renderable.hpp"
namespace legion { using namespace core; }
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
        prot.members.emplace("material", member_value("material", make_prototype(obj.material)));
        return prot;
    }
}
