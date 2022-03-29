#include "autogen_reflector_light_data.hpp"
#include "../../rendering/components/light.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::rendering::detail::light_data>(legion::rendering::detail::light_data& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::rendering::detail::light_data>();
        refl.typeName = "legion::rendering::detail::light_data";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members = std::vector<member_reference>
        {
        member_reference
        {
            "attenuation",
            primitive_reference {typeHash<float>(), &obj.attenuation}
        }
        ,
        member_reference
        {
            "intensity",
            primitive_reference {typeHash<float>(), &obj.intensity}
        }
        ,
        member_reference
        {
            "falloff",
            primitive_reference {typeHash<float>(), &obj.falloff}
        }
        ,
        member_reference
        {
            "angle",
            primitive_reference {typeHash<float>(), &obj.angle}
        }
        };
        {
            auto nested_refl = make_reflector(obj.color);
            refl.members.emplace_back("color",nested_refl);
        }
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::rendering::detail::light_data>(const legion::rendering::detail::light_data& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::rendering::detail::light_data>();
        refl.typeName = "legion::rendering::detail::light_data";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members = std::vector<member_reference>
        {
        member_reference
        {
            "attenuation",
            primitive_reference {typeHash<float>(), &obj.attenuation}
        }
        ,
        member_reference
        {
            "intensity",
            primitive_reference {typeHash<float>(), &obj.intensity}
        }
        ,
        member_reference
        {
            "falloff",
            primitive_reference {typeHash<float>(), &obj.falloff}
        }
        ,
        member_reference
        {
            "angle",
            primitive_reference {typeHash<float>(), &obj.angle}
        }
        };
        {
            auto nested_refl = make_reflector(obj.color);
            refl.members.emplace_back("color",nested_refl);
        }
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
