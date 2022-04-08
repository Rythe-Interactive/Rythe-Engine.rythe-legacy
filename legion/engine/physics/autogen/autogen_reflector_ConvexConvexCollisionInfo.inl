#include "autogen_reflector_ConvexConvexCollisionInfo.hpp"
#include "../../physics/diviner/data/convex_convex_collision_info.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::physics::ConvexConvexCollisionInfo>(legion::physics::ConvexConvexCollisionInfo& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::physics::ConvexConvexCollisionInfo>();
        refl.typeName = "legion::physics::ConvexConvexCollisionInfo";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::physics::ConvexConvexCollisionInfo>(const legion::physics::ConvexConvexCollisionInfo& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::physics::ConvexConvexCollisionInfo>();
        refl.typeName = "legion::physics::ConvexConvexCollisionInfo";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
