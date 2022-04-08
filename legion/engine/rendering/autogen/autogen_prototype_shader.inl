#include "autogen_prototype_shader.hpp"
#include "../../rendering/data/shader.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::rendering::shader>(const legion::rendering::shader& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::rendering::shader>();
        prot.typeName = "legion::rendering::shader";
        return prot;
    }
}
