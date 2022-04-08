#include "autogen_prototype_cube_collider_params.hpp"
#include "../../physics/diviner/cube_collider_params.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::physics::cube_collider_params>(const legion::physics::cube_collider_params& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::physics::cube_collider_params>();
        prot.typeName = "legion::physics::cube_collider_params";
        return prot;
    }
}
