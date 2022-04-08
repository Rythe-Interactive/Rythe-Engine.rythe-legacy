#include "autogen_prototype_shader_handle.hpp"
#include "../../rendering/data/shader.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::rendering::shader_handle>(const legion::rendering::shader_handle& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::rendering::shader_handle>();
        prot.typeName = "legion::rendering::shader_handle";
        return prot;
    }
}
