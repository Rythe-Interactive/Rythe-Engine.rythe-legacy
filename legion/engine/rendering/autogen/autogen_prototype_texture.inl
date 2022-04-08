#include "autogen_prototype_texture.hpp"
#include "../../rendering/data/texture.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::rendering::texture>(const legion::rendering::texture& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::rendering::texture>();
        prot.typeName = "legion::rendering::texture";
        return prot;
    }
}
