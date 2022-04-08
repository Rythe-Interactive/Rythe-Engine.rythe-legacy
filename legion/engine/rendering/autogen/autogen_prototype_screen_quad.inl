#include "autogen_prototype_screen_quad.hpp"
#include "../../rendering/data/screen_quad.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::rendering::screen_quad>(const legion::rendering::screen_quad& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::rendering::screen_quad>();
        prot.typeName = "legion::rendering::screen_quad";
        return prot;
    }
}
