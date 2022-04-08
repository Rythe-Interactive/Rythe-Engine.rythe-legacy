#include "autogen_prototype_EdgeLabel.hpp"
#include "../../physics/diviner/data/edge_label.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::physics::EdgeLabel>(const legion::physics::EdgeLabel& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::physics::EdgeLabel>();
        prot.typeName = "legion::physics::EdgeLabel";
        return prot;
    }
}
