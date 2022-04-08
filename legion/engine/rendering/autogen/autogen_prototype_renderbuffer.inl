#include "autogen_prototype_renderbuffer.hpp"
#include "../../rendering/data/renderbuffer.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::rendering::renderbuffer>(const legion::rendering::renderbuffer& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::rendering::renderbuffer>();
        prot.typeName = "legion::rendering::renderbuffer";
        return prot;
    }
}
