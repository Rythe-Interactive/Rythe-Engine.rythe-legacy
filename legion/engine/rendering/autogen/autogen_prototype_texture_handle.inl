#include "autogen_prototype_texture_handle.hpp"
#include "../../rendering/data/texture.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::rendering::texture_handle>(const legion::rendering::texture_handle& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::rendering::texture_handle>();
        prot.typeName = "legion::rendering::texture_handle";
        return prot;
    }
}
