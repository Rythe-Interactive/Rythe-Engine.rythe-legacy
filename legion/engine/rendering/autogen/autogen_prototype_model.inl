#include "autogen_prototype_model.hpp"
#include "../../rendering/data/model.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::rendering::model>(const legion::rendering::model& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::rendering::model>();
        prot.typeName = "legion::rendering::model";
        return prot;
    }
}
