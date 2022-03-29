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
            static const ::reflectable_attribute reflectable_attr{};
            prot.attributes.push_back(std::cref(reflectable_attr));
        }
        prot.members = std::vector<member_value>{
        member_value
        {
            "attenuation",
            primitive_value {typeHash<float>(),std::make_any<float>(obj.attenuation)}
        }
        ,
        member_value
        {
            "intensity",
            primitive_value {typeHash<float>(),std::make_any<float>(obj.intensity)}
        }
        ,
        member_value
        {
            "falloff",
            primitive_value {typeHash<float>(),std::make_any<float>(obj.falloff)}
        }
        ,
        member_value
        {
            "angle",
            primitive_value {typeHash<float>(),std::make_any<float>(obj.angle)}
        }
        };
        {
            auto nested_prot = make_prototype(obj.color);
            prot.members.emplace_back("color",nested_prot);
        }
        return prot;
    }
}
