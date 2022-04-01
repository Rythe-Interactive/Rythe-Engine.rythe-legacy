#include "prototype_light.hpp"
#include "../../../rendering/components/light.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::rendering::light>(const legion::rendering::light& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::rendering::light>();
        prot.typeName = "legion::rendering::light";
        {
            static const legion::reflectable_attribute reflectable_attr{};
            prot.attributes.push_back(std::cref(reflectable_attr));
        }
        prot.members.emplace("m_lightData", member_value("m_lightData", make_prototype(obj.m_lightData)));
        return prot;
    }
}
