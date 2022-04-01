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
        prot.members.emplace("attenuation", member_value("attenuation", primitive_value{typeHash<float>(), std::make_any<float>(obj.attenuation)}));
        prot.members.emplace("intensity", member_value("intensity", primitive_value{typeHash<float>(), std::make_any<float>(obj.intensity)}));
        prot.members.emplace("falloff", member_value("falloff", primitive_value{typeHash<float>(), std::make_any<float>(obj.falloff)}));
        prot.members.emplace("angle", member_value("angle", primitive_value{typeHash<float>(), std::make_any<float>(obj.angle)}));
        prot.members.emplace("color", member_value("color", prototype(typeHash<decltype(obj.color)>(), nameOfType<decltype(obj.color)>(), prototype::member_container())));
        return prot;
    }
}
