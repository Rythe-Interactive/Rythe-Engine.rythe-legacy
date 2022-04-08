#include "autogen_prototype_texture_data.hpp"
#include "../../rendering/data/texture.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::rendering::texture_data>(const legion::rendering::texture_data& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::rendering::texture_data>();
        prot.typeName = "legion::rendering::texture_data";
        return prot;
    }
}
