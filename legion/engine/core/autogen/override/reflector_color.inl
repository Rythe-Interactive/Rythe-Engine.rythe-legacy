#include "reflector_color.hpp"
#include "../../../core/math/color.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::math::color>(legion::core::math::color& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::math::color>();
        refl.typeName = "legion::core::math::color";
        refl.data = std::addressof(obj);
        refl.members.emplace("r", member_reference("r", primitive_reference{ typeHash<float>(), &obj.r }));
        refl.members.emplace("g", member_reference("g", primitive_reference{ typeHash<float>(), &obj.g }));
        refl.members.emplace("b", member_reference("b", primitive_reference{ typeHash<float>(), &obj.b }));
        refl.members.emplace("a", member_reference("a", primitive_reference{ typeHash<float>(), &obj.a }));
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::math::color>(const legion::core::math::color& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::math::color>();
        refl.typeName = "legion::core::math::color";
        refl.members.emplace("r", member_reference("r", primitive_reference{ typeHash<float>(), &obj.r }));
        refl.members.emplace("g", member_reference("g", primitive_reference{ typeHash<float>(), &obj.g }));
        refl.members.emplace("b", member_reference("b", primitive_reference{ typeHash<float>(), &obj.b }));
        refl.members.emplace("a", member_reference("a", primitive_reference{ typeHash<float>(), &obj.a }));
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
