#include "autogen_prototype_model_handle.hpp"
#include "../../rendering/data/model.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::rendering::model_handle>(const legion::rendering::model_handle& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::rendering::model_handle>();
        prot.typeName = "legion::rendering::model_handle";
        return prot;
    }
}
