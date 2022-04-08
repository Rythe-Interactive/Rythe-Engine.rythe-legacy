#include "autogen_reflector_plane.hpp"
#include "../../core/math/geometry.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::math::plane>(legion::core::math::plane& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::math::plane>();
        refl.typeName = "legion::core::math::plane";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::math::plane>(const legion::core::math::plane& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::math::plane>();
        refl.typeName = "legion::core::math::plane";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
