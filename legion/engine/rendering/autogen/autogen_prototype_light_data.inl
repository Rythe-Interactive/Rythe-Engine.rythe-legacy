#include "autogen_prototype_light_data.hpp"
#include "../../rendering/components/light.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::rendering::detail::light_data>(const legion::rendering::detail::light_data& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::rendering::detail::light_data>();
        prot.typeName = "legion::rendering::detail::light_data";
        {
            static const reflectable_attribute reflectable_attr{};
            prot.attributes.push_back(std::cref(reflectable_attr));
        }
        prot.members.emplace("type", member_value("type", primitive_value{typeHash(obj.type), std::any(obj.type)}));
        prot.members.emplace("attenuation", member_value("attenuation", primitive_value{typeHash(obj.attenuation), std::any(obj.attenuation)}));
        prot.members.emplace("intensity", member_value("intensity", primitive_value{typeHash(obj.intensity), std::any(obj.intensity)}));
        prot.members.emplace("index", member_value("index", primitive_value{typeHash(obj.index), std::any(obj.index)}));
        prot.members.emplace("direction", member_value("direction", make_prototype(obj.direction)));
        prot.members.emplace("falloff", member_value("falloff", primitive_value{typeHash(obj.falloff), std::any(obj.falloff)}));
        prot.members.emplace("position", member_value("position", make_prototype(obj.position)));
        prot.members.emplace("angle", member_value("angle", primitive_value{typeHash(obj.angle), std::any(obj.angle)}));
        prot.members.emplace("color", member_value("color", make_prototype(obj.color)));
        return prot;
    }
}
