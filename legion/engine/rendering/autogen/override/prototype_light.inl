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
        prot.members.emplace("type", member_value("type", primitive_value{ typeHash(obj.m_type), std::any(obj.m_type) }));
        prot.members.emplace("attenuation", member_value("attenuation", primitive_value{ typeHash(obj.m_attenuation), std::any(obj.m_attenuation) }));
        prot.members.emplace("intensity", member_value("intensity", primitive_value{ typeHash(obj.m_intensity), std::any(obj.m_intensity) }));
        prot.members.emplace("index", member_value("index", primitive_value{ typeHash(obj.m_index), std::any(obj.m_index) }));
        prot.members.emplace("direction", member_value("direction", make_prototype(obj.m_direction)));
        prot.members.emplace("falloff", member_value("falloff", primitive_value{ typeHash(obj.m_falloff), std::any(obj.m_falloff) }));
        prot.members.emplace("position", member_value("position", make_prototype(obj.m_position)));
        prot.members.emplace("angle", member_value("angle", primitive_value{ typeHash(obj.m_angle), std::any(obj.m_angle) }));
        prot.members.emplace("color", member_value("color", make_prototype(obj.m_color)));
        return prot;
    }
}
