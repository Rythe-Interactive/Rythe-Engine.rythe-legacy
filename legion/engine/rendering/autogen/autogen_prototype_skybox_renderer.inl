#include "autogen_prototype_skybox_renderer.hpp"
#include "../../rendering/components/renderable.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::rendering::skybox_renderer>(const legion::rendering::skybox_renderer& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::rendering::skybox_renderer>();
        prot.typeName = "legion::rendering::skybox_renderer";
        {
            static const ::reflectable_attribute reflectable_attr{};
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
