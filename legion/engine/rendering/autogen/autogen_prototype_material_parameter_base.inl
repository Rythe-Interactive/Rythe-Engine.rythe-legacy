#include "autogen_prototype_material_parameter_base.hpp"
#include "../../rendering/data/material.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::rendering::material_parameter_base>(const legion::rendering::material_parameter_base& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::rendering::material_parameter_base>();
        prot.typeName = "legion::rendering::material_parameter_base";
        return prot;
    }
}
