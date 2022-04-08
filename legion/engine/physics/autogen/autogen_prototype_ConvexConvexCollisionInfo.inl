#include "autogen_prototype_ConvexConvexCollisionInfo.hpp"
#include "../../physics/diviner/data/convex_convex_collision_info.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::physics::ConvexConvexCollisionInfo>(const legion::physics::ConvexConvexCollisionInfo& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::physics::ConvexConvexCollisionInfo>();
        prot.typeName = "legion::physics::ConvexConvexCollisionInfo";
        return prot;
    }
}
