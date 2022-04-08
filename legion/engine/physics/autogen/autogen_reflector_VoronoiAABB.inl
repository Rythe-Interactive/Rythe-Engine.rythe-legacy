#include "autogen_reflector_VoronoiAABB.hpp"
#include "../../physics/diviner/data/VoronoiAABB.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD reflector make_reflector<legion::physics::VoronoiAABB>(legion::physics::VoronoiAABB& obj)
    {
        reflector refl;
        refl.typeId = typeHash<legion::physics::VoronoiAABB>();
        refl.typeName = "legion::physics::VoronoiAABB";
        refl.data = std::addressof(obj);
        return refl;
    }
    template<>
    L_NODISCARD const reflector make_reflector<const legion::physics::VoronoiAABB>(const legion::physics::VoronoiAABB& obj)
    {
        ptr_type address = reinterpret_cast<ptr_type>(std::addressof(obj));
        reflector refl;
        refl.typeId = typeHash<legion::physics::VoronoiAABB>();
        refl.typeName = "legion::physics::VoronoiAABB";
        refl.data = reinterpret_cast<void*>(address);
        return refl;
    }
}
