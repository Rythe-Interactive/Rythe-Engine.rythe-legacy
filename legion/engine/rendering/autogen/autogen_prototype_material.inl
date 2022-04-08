#include "autogen_prototype_material.hpp"
#include "../../rendering/data/material.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::rendering::material>(const legion::rendering::material& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::rendering::material>();
        prot.typeName = "legion::rendering::material";
        return prot;
    }
}
