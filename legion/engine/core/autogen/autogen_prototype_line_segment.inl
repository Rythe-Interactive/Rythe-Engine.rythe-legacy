#include "autogen_prototype_line_segment.hpp"
#include "../../core/math/geometry.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::math::line_segment>(const legion::core::math::line_segment& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::math::line_segment>();
        prot.typeName = "legion::core::math::line_segment";
        return prot;
    }
}
