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
        refl.members.emplace("attenuation", member_reference("attenuation", primitive_reference{typeHash<float>(), &obj.attenuation}));
        refl.members.emplace("intensity", member_reference("intensity", primitive_reference{typeHash<float>(), &obj.intensity}));
        refl.members.emplace("falloff", member_reference("falloff", primitive_reference{typeHash<float>(), &obj.falloff}));
        refl.members.emplace("angle", member_reference("angle", primitive_reference{typeHash<float>(), &obj.angle}));
        refl.members.emplace("color", member_reference("color", reflector(typeHash<decltype(obj.color)>(), nameOfType<decltype(obj.color)>(), reflector::member_container(), std::addressof(obj.color))));
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
        refl.members.emplace("attenuation", member_reference("attenuation", primitive_reference{typeHash<float>(), &obj.attenuation}));
        refl.members.emplace("intensity", member_reference("intensity", primitive_reference{typeHash<float>(), &obj.intensity}));
        refl.members.emplace("falloff", member_reference("falloff", primitive_reference{typeHash<float>(), &obj.falloff}));
        refl.members.emplace("angle", member_reference("angle", primitive_reference{typeHash<float>(), &obj.angle}));
        refl.members.emplace("color", member_reference("color", reflector(typeHash<decltype(obj.color)>(), nameOfType<decltype(obj.color)>(), reflector::member_container(), std::addressof(obj.color))));
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
