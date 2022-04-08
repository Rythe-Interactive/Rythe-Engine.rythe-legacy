#include "autogen_prototype_triangle.hpp"
#include "../../core/math/geometry.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::math::triangle>(const legion::core::math::triangle& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::math::triangle>();
        prot.typeName = "legion::core::math::triangle";
        return prot;
    }
}
