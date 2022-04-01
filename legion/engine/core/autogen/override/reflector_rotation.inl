#include "reflector_rotation.hpp"
#include "../../../core/defaults/defaultcomponents.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::rotation>(legion::core::rotation& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::rotation>();
        refl.typeName = "legion::core::rotation";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members.emplace("x", member_reference("x", primitive_reference{ typeHash<float>(), &obj.x }));
        refl.members.emplace("y", member_reference("y", primitive_reference{ typeHash<float>(), &obj.y }));
        refl.members.emplace("z", member_reference("z", primitive_reference{ typeHash<float>(), &obj.z }));
        refl.members.emplace("w", member_reference("w", primitive_reference{ typeHash<float>(), &obj.w }));
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::rotation>(const legion::core::rotation& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::rotation>();
        refl.typeName = "legion::core::rotation";
        {
            static const reflectable_attribute reflectable_attr{};
            refl.attributes.push_back(std::cref(reflectable_attr));
        }
        refl.members.emplace("x", member_reference("x", primitive_reference{ typeHash<float>(), &obj.x }));
        refl.members.emplace("y", member_reference("y", primitive_reference{ typeHash<float>(), &obj.y }));
        refl.members.emplace("z", member_reference("z", primitive_reference{ typeHash<float>(), &obj.z }));
        refl.members.emplace("w", member_reference("w", primitive_reference{ typeHash<float>(), &obj.w }));
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
