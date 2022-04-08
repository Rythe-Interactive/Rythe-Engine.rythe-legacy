#include "autogen_prototype_shader_parameter_base.hpp"
#include "../../rendering/data/shader.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::rendering::shader_parameter_base>(const legion::rendering::shader_parameter_base& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::rendering::shader_parameter_base>();
        prot.typeName = "legion::rendering::shader_parameter_base";
        return prot;
    }
}
