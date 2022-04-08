#include "autogen_prototype_shader_variant.hpp"
#include "../../rendering/data/shader.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::rendering::shader_variant>(const legion::rendering::shader_variant& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::rendering::shader_variant>();
        prot.typeName = "legion::rendering::shader_variant";
        return prot;
    }
}
