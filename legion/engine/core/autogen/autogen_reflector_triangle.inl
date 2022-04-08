#include "autogen_reflector_triangle.hpp"
#include "../../core/math/geometry.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::math::triangle>(legion::core::math::triangle& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::math::triangle>();
        refl.typeName = "legion::core::math::triangle";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::math::triangle>(const legion::core::math::triangle& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::math::triangle>();
        refl.typeName = "legion::core::math::triangle";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
