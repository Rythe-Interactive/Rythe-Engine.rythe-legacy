#include "autogen_prototype_material_handle.hpp"
#include "../../rendering/data/material.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::rendering::material_handle>(const legion::rendering::material_handle& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::rendering::material_handle>();
        prot.typeName = "legion::rendering::material_handle";
        return prot;
    }
}
