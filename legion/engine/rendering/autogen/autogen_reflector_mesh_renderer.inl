#include "autogen_reflector_mesh_renderer.hpp"
#include "../../rendering/components/renderable.hpp"
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::rendering::mesh_renderer>(legion::rendering::mesh_renderer& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::rendering::mesh_renderer>();
        refl.typeName = "legion::rendering::mesh_renderer";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members = std::vector<member_reference>
        ();
        {
            auto nested_refl = make_reflector(obj.material);
            refl.members.emplace_back("material",nested_refl);
        }
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::rendering::mesh_renderer>(const legion::rendering::mesh_renderer& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::rendering::mesh_renderer>();
        refl.typeName = "legion::rendering::mesh_renderer";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members = std::vector<member_reference>
        ();
        {
            auto nested_refl = make_reflector(obj.material);
            refl.members.emplace_back("material",nested_refl);
        }
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
