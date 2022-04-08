#include "autogen_prototype_attribute.hpp"
#include "../../rendering/data/shader.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::rendering::attribute>(const legion::rendering::attribute& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::rendering::attribute>();
        prot.typeName = "legion::rendering::attribute";
        return prot;
    }
}
