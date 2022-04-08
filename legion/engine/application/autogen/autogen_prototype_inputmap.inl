#include "autogen_prototype_inputmap.hpp"
#include "../../application/input/inputmap.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::application::inputmap>(const legion::application::inputmap& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::application::inputmap>();
        prot.typeName = "legion::application::inputmap";
        return prot;
    }
}
