#include "autogen_reflector_skybox_renderer.hpp"
#include "../../rendering/components/renderable.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::rendering::skybox_renderer>(legion::rendering::skybox_renderer& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::rendering::skybox_renderer>();
        refl.typeName = "legion::rendering::skybox_renderer";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members.emplace("material", member_reference("material", make_reflector(obj.material)));
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::rendering::skybox_renderer>(const legion::rendering::skybox_renderer& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::rendering::skybox_renderer>();
        refl.typeName = "legion::rendering::skybox_renderer";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members.emplace("material", member_reference("material", make_reflector(obj.material)));
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
