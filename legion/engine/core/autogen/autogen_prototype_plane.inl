#include "autogen_prototype_plane.hpp"
#include "../../core/math/geometry.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::math::plane>(const legion::core::math::plane& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::math::plane>();
        prot.typeName = "legion::core::math::plane";
        return prot;
    }
}
