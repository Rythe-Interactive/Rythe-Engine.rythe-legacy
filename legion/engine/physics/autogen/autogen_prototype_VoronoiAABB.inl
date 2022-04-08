#include "autogen_prototype_VoronoiAABB.hpp"
#include "../../physics/diviner/data/VoronoiAABB.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::physics::VoronoiAABB>(const legion::physics::VoronoiAABB& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::physics::VoronoiAABB>();
        prot.typeName = "legion::physics::VoronoiAABB";
        return prot;
    }
}
