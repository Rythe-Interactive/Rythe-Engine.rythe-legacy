#include "autogen_prototype_vertexarray.hpp"
#include "../../rendering/data/vertexarray.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::rendering::vertexarray>(const legion::rendering::vertexarray& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::rendering::vertexarray>();
        prot.typeName = "legion::rendering::vertexarray";
        return prot;
    }
}
