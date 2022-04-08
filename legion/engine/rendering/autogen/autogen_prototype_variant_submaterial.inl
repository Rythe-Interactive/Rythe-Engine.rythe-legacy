#include "autogen_prototype_variant_submaterial.hpp"
#include "../../rendering/data/material.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::rendering::variant_submaterial>(const legion::rendering::variant_submaterial& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::rendering::variant_submaterial>();
        prot.typeName = "legion::rendering::variant_submaterial";
        return prot;
    }
}
