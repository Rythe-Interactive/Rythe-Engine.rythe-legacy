#include "autogen_prototype_framebuffer.hpp"
#include "../../rendering/data/framebuffer.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::rendering::framebuffer>(const legion::rendering::framebuffer& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::rendering::framebuffer>();
        prot.typeName = "legion::rendering::framebuffer";
        return prot;
    }
}
