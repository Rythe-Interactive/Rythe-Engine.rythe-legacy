#include "autogen_prototype_ShaderCache.hpp"
#include "../../rendering/data/shader.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::rendering::ShaderCache>(const legion::rendering::ShaderCache& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::rendering::ShaderCache>();
        prot.typeName = "legion::rendering::ShaderCache";
        return prot;
    }
}
