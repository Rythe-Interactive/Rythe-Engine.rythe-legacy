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
        refl.members.emplace("m_lightData", member_reference("m_lightData", make_reflector(obj.m_lightData)));
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
        refl.members.emplace("m_lightData", member_reference("m_lightData", make_reflector(obj.m_lightData)));
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
