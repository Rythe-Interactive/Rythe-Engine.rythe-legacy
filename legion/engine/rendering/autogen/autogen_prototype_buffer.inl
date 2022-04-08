#include "autogen_prototype_buffer.hpp"
#include "../../rendering/data/buffer.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::rendering::buffer>(const legion::rendering::buffer& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::rendering::buffer>();
        prot.typeName = "legion::rendering::buffer";
        return prot;
    }
}
