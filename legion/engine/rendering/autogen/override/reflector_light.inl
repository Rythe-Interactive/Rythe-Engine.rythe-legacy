#include "reflector_light.hpp"
#include "../../../rendering/components/light.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::rendering::light>(legion::rendering::light& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::rendering::light>();
        refl.typeName = "legion::rendering::light";
        {
            static const legion::reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members.emplace("type", member_reference("type", primitive_reference{ typeHash(obj.m_type), &obj.m_type }));
        refl.members.emplace("attenuation", member_reference("attenuation", primitive_reference{ typeHash(obj.m_attenuation), &obj.m_attenuation }));
        refl.members.emplace("intensity", member_reference("intensity", primitive_reference{ typeHash(obj.m_intensity), &obj.m_intensity }));
        refl.members.emplace("index", member_reference("index", primitive_reference{ typeHash(obj.m_index), &obj.m_index }));
        refl.members.emplace("direction", member_reference("direction", make_reflector(obj.m_direction)));
        refl.members.emplace("falloff", member_reference("falloff", primitive_reference{ typeHash(obj.m_falloff), &obj.m_falloff }));
        refl.members.emplace("position", member_reference("position", make_reflector(obj.m_position)));
        refl.members.emplace("angle", member_reference("angle", primitive_reference{ typeHash(obj.m_angle), &obj.m_angle }));
        refl.members.emplace("color", member_reference("color", make_reflector(obj.m_color)));
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::rendering::light>(const legion::rendering::light& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::rendering::light>();
        refl.typeName = "legion::rendering::light";
        {
            static const legion::reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members.emplace("type", member_reference("type", primitive_reference{ typeHash(obj.m_type), &obj.m_type }));
        refl.members.emplace("attenuation", member_reference("attenuation", primitive_reference{ typeHash(obj.m_attenuation), &obj.m_attenuation }));
        refl.members.emplace("intensity", member_reference("intensity", primitive_reference{ typeHash(obj.m_intensity), &obj.m_intensity }));
        refl.members.emplace("index", member_reference("index", primitive_reference{ typeHash(obj.m_index), &obj.m_index }));
        refl.members.emplace("direction", member_reference("direction", make_reflector(obj.m_direction)));
        refl.members.emplace("falloff", member_reference("falloff", primitive_reference{ typeHash(obj.m_falloff), &obj.m_falloff }));
        refl.members.emplace("position", member_reference("position", make_reflector(obj.m_position)));
        refl.members.emplace("angle", member_reference("angle", primitive_reference{ typeHash(obj.m_angle), &obj.m_angle }));
        refl.members.emplace("color", member_reference("color", make_reflector(obj.m_color)));
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
