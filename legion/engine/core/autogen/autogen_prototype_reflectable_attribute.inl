#include "autogen_prototype_reflectable_attribute.hpp"
#include "../../core/types/attributes.hpp"
namespace legion { using namespace core; }
namespace legion::core
{
    template<>
    L_NODISCARD prototype make_prototype<legion::core::reflectable_attribute>(const legion::core::reflectable_attribute& obj)
    {
        prototype prot;
        prot.typeId = typeHash<legion::core::reflectable_attribute>();
        prot.typeName = "legion::core::reflectable_attribute";
        return prot;
    }
}
