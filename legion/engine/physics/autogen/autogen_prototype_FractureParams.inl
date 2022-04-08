#include "autogen_prototype_FractureParams.hpp"
#include "../../physics/diviner/data/fractureparams.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::physics::FractureParams>(const legion::physics::FractureParams& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::physics::FractureParams>();
        prot.typeName = "legion::physics::FractureParams";
        return prot;
    }
}
