#include "autogen_reflector_line_segment.hpp"
#include "../../core/math/geometry.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::core::math::line_segment>(legion::core::math::line_segment& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::core::math::line_segment>();
        refl.typeName = "legion::core::math::line_segment";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::core::math::line_segment>(const legion::core::math::line_segment& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::core::math::line_segment>();
        refl.typeName = "legion::core::math::line_segment";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
