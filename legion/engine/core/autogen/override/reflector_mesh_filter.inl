#include "reflector_mesh_filter.hpp"
#include "../../../core/defaults/defaultcomponents.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::mesh_filter>(legion::core::mesh_filter& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::mesh_filter>();
        refl.typeName = "legion::core::mesh_filter";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members.emplace("shared_mesh", member_reference("shared_mesh", make_reflector(obj.shared_mesh)));
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::mesh_filter>(const legion::core::mesh_filter& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::mesh_filter>();
        refl.typeName = "legion::core::mesh_filter";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members.emplace("shared_mesh", member_reference("shared_mesh", make_reflector(obj.shared_mesh)));
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
